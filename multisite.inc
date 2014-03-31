<?php

/**
 * Multisite entity class.
 */
class Multisite extends Entity {
  /**
   * Override defaultUri().
   */
  protected function defaultUri() {
    return array('path' => 'http://' . $this->hostname, 'options' => array('external' => TRUE));
  }
}

/**
 * Multisite entity controller for managing multisites.
 */
class MultisiteController extends EntityAPIController {
  /**
   * Setup the multisite by scaffolding the settings file and creating/renaming
   * the database.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $hostname = $entity->hostname;
    $sites_path = DRUPAL_ROOT . "/sites/$hostname";
    $settings_path = "$sites_path/settings.php";
    $settings_default = variable_get('multisite_default_settings', 'sites/default/default.settings.php');
    $database_name = multisite_get_database_name($hostname);
    $is_new = isset($entity->is_new);

    // Make sure default settings file is readable.
    if (!is_readable($settings_default)) {
      throw new Exception('Could not read default settings file');
    }

    // For new sites, create a multisite folder in the sites directory and
    // create the sites database.
    if ($is_new) {
      // Make sure the sites folder is writable so the multisite directory can
      // be created.
      if (!is_writable(dirname($sites_path))) {
        throw new Exception('Could not create multisite directory');
      }
      $entity->created = REQUEST_TIME;
      // Create the database as all filesystem checks have been done.
      db_query("CREATE DATABASE $database_name");
      // Create the multisite directory.
      if (!drupal_mkdir($sites_path)) {
        throw new Exception('Could not create multisite directory');
      }
    }
    // For existing multisites, rename the multisite folder as well as the
    // database.
    else {
      $old = multisite_load($entity->msid);
      // If the sitename has changed.
      if ($old->name != $entity->name) {
        multisite_reconnect_database($old->hostname);
        variable_set('site_name', $entity->name);
        multisite_reconnect_database();
      }
      // If the hostname has changed, rename the database and the multisite
      // folder.
      if ($old->hostname != $entity->hostname) {
        $entity->database_name = $database_name;
        $old_sites_path = DRUPAL_ROOT . 'sites/' . $old->hostname;
        $old_database_name = multisite_get_database_name($old->hostname);
        // Make sure the multisite directory is writable
        if (!is_readable($sites_path) || !is_writable(dirname($sites_path)) || file_exists($sites_path)) {
          throw new Exception('Multisite directory is not writable');
        }
        // Rename the database as the filesystem checks have been done.
        db_query("RENAME DATABASE $old_database_name TO $database_name");
        // Rename the sites directory.
        if (!rename($old_sites_path, $sites_path)) {
          throw new Exception('Could not rename multisite directory');
        }
      }
    }

    $entity->changed = REQUEST_TIME;

    // Scaffold the settings file on each save.
    $this->scaffoldSettingsFile($entity, $settings_path, $database_name);

    // Save the actual entity
    $result = parent::save($entity, $transaction);

    // For new multisites redirect to the install.php script.
    if ($is_new) {
      $this->gotoInstallation($entity);
    }
    return $result;
  }

  /**
   * Scaffold the settings file, this should be done on each save to make sure
   * it's up to date.
   */
  protected function scaffoldSettingsFile($entity, $settings_path, $database_name) {
    $hostname = $entity->hostname;
    $subdomain = multisite_get_subdomain($hostname);
    $settings_default = variable_get('multisite_default_settings', 'sites/default/default.settings.php');
    $settings_contents = file_get_contents($settings_default);

    // Replace the database name and generate a hash salt.
    // @see install_settings_form_submit().
    // We cannot use drupal_rewrite_settings() as it's locked to the current
    // working sites directory.
    $hash = drupal_random_key();
    $settings_contents .= <<<EOT
\$databases['default']['default']['database'] = '$database_name';
// On installation and update runs separate tables are required.
if (preg_match('@^/(install|update)\.php@', request_uri(), \$matches) && !isset(\$install_done)) {
  \$databases['default']['default']['prefix'] = '${database_name}.';
}
// Once installed, use the shared tables.
else {
  \$databases['default']['default']['prefix']['default'] = '${database_name}.';
}
\$drupal_hash_salt = '$hash';
EOT;
    if (file_exists($settings_path) && !is_writable($settings_path)) {
      drupal_chmod($settings_path, 0644);
    }
    file_put_contents($settings_path, $settings_contents);
  }

  /**
   * Initialize the installation by storing an entry in the prcess table so
   * that the multisite can be restored in the installation profile.
   */
  protected function gotoInstallation($multisite) {
    global $user, $cookie_domain;
    $key = drupal_substr(drupal_random_key(), 0, 31);
    $hostname = $multisite->hostname;
    $is_authenticated = $user->uid > 1;

    setcookie('multisite_process_key', $key, 0, '/', $cookie_domain);
    db_insert('multisite_process')
      ->fields(array(
        'uid' => $user->uid,
        'msid' => $multisite->msid,
        'private_key' => $key,
        'created' => REQUEST_TIME,
      ))
      ->execute();

    drupal_goto("http://$hostname/install.php", array(
      'external' => TRUE,
      'query' => array(
        'locale' => $multisite->language,
        'profile' => $multisite->profile,
        // If the user is authenticated but not admin, use that user i the
        // installation form instead o creating a new one.
        'create_user' => $is_authenticated ? 'false' : 'true',
        'site_name' => urlencode(check_plain($multisite->name)),
      ),
    ));
  }

  /**
   * Drop the database and recursively delete the sites folder.
   * @TODO www-data doesnt have access to remove the settings file.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    global $databases;
    $multisites = multisite_load_multiple($ids);
    foreach ($multisites as $multisite) {
      $database = db_escape_field($multisite->database_name);
      $hostname = $multisite->hostname;
      // Make sure we're not under any circumstances dropping the main
      // database.
      if ($database != $databases['default']['default']['database']) {
        try {
          db_query("DROP DATABASE $database");
        }
        // Maybe the database never existed, ignore
        catch(Exception $e) {
          drupal_set_message(t('The database @database could not be deleted', array('@database' => $database)), 'warning');
        }
      }
      else {
        drupal_set_message(t('The database @database is  not allowed to be deleted', array('@database' => $database)), 'warning');
      }
      if ($sites_path = realpath(DRUPAL_ROOT . "/sites/$hostname")) {
        // Make sure we're not under any circumstances deleting the all or
        // default directories.
        if (!in_array($sites_path, array(DRUPAL_ROOT . '/sites/all', DRUPAL_ROOT . '/sites/default'))) {
          file_unmanaged_delete_recursive($sites_path);
        }
        else {
          drupal_set_message(t('The @dir directory is not allowed to be deleted', array('@dir' => $sites_path)), 'warning');
        }
      }
      else {
        drupal_set_message(t('The multisite directory does not exist'), 'warning');
      }
    }
    parent::delete($ids, $transaction);
  }
}

/**
 * Admin UI Controller mostly overriden by views.
 */
class MultisiteUIController extends EntityDefaultUIController {
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['title'] = t('Multisites');
    $items[$this->path]['description'] = t('Manage multisites and their fields');
    $items[$this->path]['access callback'] = 'user_access';
    $items[$this->path]['access arguments'] = array('administer multisite entities');
    return $items;
  }
}

