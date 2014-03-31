<?php
/**
 * @file
 * This file should be included in your custom profile and then setup with your own hooks.
 *
 * The following two hooks must be implemented and called by your own profile:
 * - hook_install_tasks().
 * - hook_form_install_configure_form_alter().
 *
 */


/**
 * Implements hook_install_tasks().
 */
function multisite_profile_install_tasks(&$install_state) {
  $tasks['multisite_profile_configure_form'] = array(
    'display_name' => 'Site configuration',
    'type' => 'form',
  );
  return $tasks;
}

/**
 * Implements hook_form_install_configure_form_alter().
 */
function multisite_profile_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = isset($_GET['site_name']) ? check_plain(urldecode($_GET['site_name'])) : $_SERVER['SERVER_NAME'];

  // Hide some messages
  drupal_get_messages('status');
  drupal_get_messages('warning');

  // If multisite was initialized with a valid user that we can use for
  // admin account.
  if (!empty($_GET['create_user']) && $_GET['create_user'] == 'false' && isset($_COOKIE['multisite_process_key'])) {
    $form['admin_account']['#access'] = FALSE;
    $form['admin_account']['account']['name']['#type'] = 'hidden';
    $form['admin_account']['account']['mail']['#type'] = 'hidden';
    $form['admin_account']['account']['pass']['#type'] = 'hidden';
    $form['admin_account']['account']['name']['#value'] = 'admin';
    $form['admin_account']['account']['mail']['#value'] = variable_get('multisite_temp_email', 'admin@example.org');
    $form['admin_account']['account']['pass']['#value'] = user_password();
  }
  // If an administrator or anonymous user initalized the prcess, let them
  // create the admin account but validate the fields in the shared tables.
  else {
    $form['admin_account']['account']['name']['#element_validate'][] = 'multisite_profile_validate_name';
    $form['admin_account']['account']['mail']['#element_validate'][] = 'multisite_profile_validate_mail';
  }

  $form['update_notifications']['#access'] = FALSE;
  $form['update_notifications']['update_status_module']['#default_value'] = array(0, 0);

  // Submit handler creating the account on the shared table.
  $form['#submit'][] = 'multisite_profile_install_configure_form_submit';
}

/**
 * Get the active multisite installation process which was initialized on the
 * main site.
 */
function _multisite_profile_get_process() {
  $key = $_COOKIE['multisite_process_key'];

  $process = db_select('multisite_process', 'mp')
    ->fields('mp')
    ->condition('private_key', $key)
    ->execute()
    ->fetchAssoc();

  return $process;
}

/**
 * Form callback rendering the multisite entity form.
 */
function multisite_profile_configure_form($form, &$form_state, &$install_state) {
  global $user;
  drupal_set_title(st('Site configuration'));

  _multisite_profile_reconnect_database();
  $process = _multisite_profile_get_process();

  $multisite = multisite_load($process['msid']);
  // If the multisite owner hasnt been set, set it now.
  if ($multisite->uid == 0) {
    $multisite->uid = $user->uid;
    $multisite->save();
  }

  // Include the entity form
  module_load_include('inc', 'multisite', 'multisite.admin');
  $form = multisite_form($form, $form_state, $multisite);
  // Restrict access to some fields.
  $form['language']['#access'] = FALSE;
  $form['profile']['#access'] = FALSE;
  $form['hostname']['#access'] = FALSE;
  // The form transforms the hostname to a subdomain for UX and then changes it
  // back in the validate function. In case there's no multisite_domain
  // variable set this will change into an incorrect value though, so disable
  // the functionality as the field is not accessable anyways.
  $form['hostname']['#default_value'] = $multisite->hostname;
  unset($form['hostname']['#element_validate']);

  // Fix up actions
  $form['actions']['submit']['#value'] = t('Save settings');
  unset($form['actions']['delete']);
  // Required by entity_ui_form_submit_build_entity().
  $form_state['entity_type'] = 'multisite';
  $form_state['multisite'] = $multisite;

  return $form;
}

/**
 * Submit handler for saving the multisite entity
 */
function multisite_profile_configure_form_submit($form, &$form_state) {
  global $databases, $user;
  _multisite_profile_reconnect_database();

  $multisite = entity_ui_form_submit_build_entity($form, $form_state);
  // Sign in as the multisites owner.
  $user = user_load($multisite->uid);
  $multisite->database_name = $databases['default']['default']['database'];
  $multisite->save();

  $key = $_COOKIE['multisite_process_key'];
  // Mark the installation process as complete
  db_update('multisite_process')
    ->fields(array(
      'completed' => REQUEST_TIME,
    ))
    ->condition('private_key', $key)
    ->execute();

  // Login as the user.
  user_login_finalize();
}


/**
 * Validate that the user name is not taken in the shared user table.
 */
function multisite_profile_validate_name($element, &$form_state, $form) {
  $value = $element['#value'];
  // Shared users table, should be set in the settings.php file
  $user_table = variable_get('multisite_user_table', '{users}');
  $query = db_query("SELECT * FROM $user_table WHERE name = :name", array(':name' => $value));
  if ($query->rowCount() > 0) {
    form_error($element, t('The username is already taken.'));
  }
}

/**
 * Validate that the user email is not taken in the shared user table.
 */
function multisite_profile_validate_mail($element, &$form_state, $form) {
  $value = $element['#value'];
  // Shared users table, should be set in the settings.php file
  $user_table = variable_get('multisite_user_table', '{users}');
  $query = db_query("SELECT * FROM $user_table WHERE mail = :mail", array(':mail' => $value));
  if ($query->rowCount() > 0) {
    form_error($element, t('The e-mail address is already taken.'));
  }
}

/**
 * Reconnect to the database with the shared settings set.
 * @TODO this should be cleaned up to use db_set_active().
 */
function _multisite_profile_reconnect_database() {
  global $install_done, $databases;
  // Mark the installation as done so settings.php picks up the prefixed tables.
  $install_done = TRUE;
  // Override the $databases array.
  include conf_path() . '/settings.php';
  // Close the current connection and reparse the $databases array.
  db_close();
  Database::parseConnectionInfo();
}

/**
 * Callback configuring the multisites owner for the multisite instance.
 */
function multisite_profile_install_configure_form_submit($form, $form_state) {
  global $databases;
  // Load the user account from the temporary user table.
  $account = clone user_load(1);
  // Remove the uid to create a new user in the shared table.
  $account->uid = NULL;

  _multisite_profile_reconnect_database();

  // If the process was initialized by a user other than the administrator, use
  // that as the multisites account.
  $process = _multisite_profile_get_process();
  if ($process['uid'] > 1 && $initial_user = user_load($process['uid'])) {
    $account = $initial_user;
  }

  // The role for users of the new sites.
  $role = user_role_load_by_name(variable_get('multisite_admin_role', 'administrator'));

  $account->roles = array($role->rid => $role);
  // Delete admin accounts paths as they use the same account name and therefore
  // take up the namespace user_save would use.
  path_delete(array('source' => 'user/1'));
  path_delete(array('source' => 'blog/1'));
  // Save the new account into the shared user table.
  user_save($account);

  // Login as the correct user.
  global $user;
  $user = $account;
  user_login_finalize();
}
