<?php

/**
 * Add/edit form for multisite entities.
 */
function multisite_form($form, &$form_state, $multisite = NULL) {
  $is_new = !isset($multisite->msid);

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name'),
    '#description' => t('The name of the multisite.'),
    '#default_value' => !$is_new ? $multisite->name : '',
    '#required' => TRUE,
  );
  // User inputs the hostname as a subdomain which is then changed to the
  // hostname on validation.
  $form['hostname'] = array(
    '#type' => 'textfield',
    '#title' => t('Subdomain'),
    '#description' => t('The subdomain of this multisite.'),
    '#suffix' => multisite_get_domain(''),
    '#element_validate' => array('_multisite_hostname_validate'),
    '#default_value' => !$is_new ? multisite_get_subdomain($multisite->hostname) : '',
  );

  // Fetch valid languages for the selected profile.
  include_once DRUPAL_ROOT . '/includes/install.core.inc';
  $profile = variable_get('multisite_profilename', 'standard');
  $locale_options = array();
  $locales = install_find_locales($profile);
  $languages = language_list();
  foreach ($locales as $locale) {
    $language = $languages[$locale->langcode]->native;
    $locale_options[$locale->langcode] = $language;
  }

  $form['profile'] = array(
    '#type' => 'hidden',
    '#value' => $is_new ? $profile : $multisite->profile,
  );

  $form['uid'] = array(
    '#type' => 'hidden',
    '#value' => $is_new ? 0 : $multisite->uid,
  );

  if ($is_new) {
    // If there are more than 1 language enabled, let the user choose which
    // language to use.
    if (count($locales) > 1) {
      $form['language'] = array(
        '#type' => 'select',
        '#title' => t('Locale'),
        '#options' => $locale_options,
      );
    }
    // If there's only one language, do not display a select list.
    else {
      $form['language'] = array(
        '#type' => 'hidden',
        '#value' => $locales[0]->name,
      );
    }
  }
  else {
    $form['language'] = array(
      '#type' => 'select',
      '#title' => t('Locale'),
      '#options' => $locale_options,
      '#default_value' => $multisite->language,
      '#disabled' => TRUE,
    );
  }

  // Attach custom fields.
  field_attach_form('multisite', $multisite, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => $is_new ? t('Save multisite') : t('Update multisite'),
    ),
    'delete' => array(
      '#markup' => !$is_new ? l(t('Delete'), 'admin/people/multisite/manage/' . $multisite->msid . '/delete', array('attributes' => array('class' => array('button remove')), 'query' => array('destination' => 'admin/people/multisite'))) : '',
    ),
  );
  return $form;
}

/**
 * Validate a hostname, this should only run during creation as update would
 * invalidate it.
 * Additionally this changes the inputted subdomain to the actual hostname.
 * @TODO this is ugly.
 */
function _multisite_hostname_validate($element, &$form_state) {
  $value = $element['#value'];
  $errors = FALSE;
  if (!preg_match('/^[a-z]+[a-z\-_]*[a-z]+$/', $value)) {
    $errors = TRUE;
    form_error($element, t('The subdomain can only use lowercased letters, dashes and underscores. It must also start and end with a letter.'));
  }
  if (in_array($value, array('www'))) {
    $errors = TRUE;
    form_error($element, t('@value is a restricted subdomain.', array('@value' => $value)));
  }
  $new_hostname = multisite_get_domain($value);
  if ($form_state['multisite']->hostname != $new_hostname) {
    $database = multisite_get_database_name($new_hostname);
    $exists = db_query('SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = :name', array(':name' => $database))->fetchField();
    if ($exists) {
      $errors = TRUE;
      form_error($element, t('The database @name already exists', array('@name' => $database)));
    }
    if (is_dir(DRUPAL_ROOT . "/sites/$new_hostname")) {
      $errors = TRUE;
      form_error($element, t('The subdomain already exists.'));
    }
  }
  if (!$errors) {
    // The hostname is input as a subdomain, append the rest.
    form_set_value($element, multisite_get_domain($value), $form_state);
  }
}

/**
 * Callback initializing the multisite creation process.
 */
function multisite_form_submit($form, &$form_state) {
  $multisite = entity_ui_form_submit_build_entity($form, $form_state);
  $multisite->save();
}


/**
 * Make sure it's safe to create this multisite.
 */
function multisite_form_validate($form, &$form_state) {
  $directory = DRUPAL_ROOT . '/sites';
  if (!file_prepare_directory($directory)) {
    form_set_error(NULL, t('The sites directory is not writable'));
  }
}

/**
 * General configuration form controlling multisite creation.
 * All of thse should be set in code with the $conf variable so that they are
 * shared across multisites.
 */
function multisite_admin_settings() {
  drupal_set_message(t('These settings should exist on multisite instances as well, thefore you should either set it thorugh the $conf variable in a shared settings file or through a feature.'), 'status');

  $form['multisite_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#default_value' => variable_get('multisite_domain', ''),
    '#description' => t('If left empty it wil default to the current site domain without the www subdomain. Other subdomains will be kept.'),
  );

  include_once DRUPAL_ROOT . '/includes/install.core.inc';
  $profiles = install_find_profiles();
  $profiles = array_keys($profiles);
  $profiles = array_combine($profiles, $profiles);

  $form['multisite_profilename'] = array(
    '#type' => 'select',
    '#title' => t('Installation profile'),
    '#description' => t('The installation profile to use when creating a multisite instance.'),
    '#default_value' => variable_get('multisite_profilename', 'standard'),
    '#options' => $profiles,
  );

  $form['multisite_default_settings'] = array(
    '#type' => 'textfield',
    '#title' => t('Settings file'),
    '#description' => t('Path to the settings.php file to use as a base for new multisites. This file should contain the database username and password prefilled! Only the database name will be replaced by the creation process.'),
    '#default_value' => variable_get('multisite_default_settings', 'sites/default/default.settings.php'),
  );

  return system_settings_form($form);
}
