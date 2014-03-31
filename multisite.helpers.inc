<?php

/**
 * Get the subdomain forom a hostname.
 */
function multisite_get_subdomain($hostname) {
  if (strpos('http', $hostname) === 0) {
    $hostname = parse_url($hostname, PHP_URL_HOST);
  }
  $hostname = explode('.', $hostname);
  return $hostname[0];
}

/**
 * Return the full domain name of a multisite based on its subdomain.
 */
function multisite_get_domain($subdomain, $strip = array('www')) {
  $domain = variable_get('multisite_domain', '');
  if (empty($domain)) {
    $host = explode('.', $_SERVER['SERVER_NAME']);
    // Shift away leading subdomain.
    if (in_array($host[0], $strip)) {
      array_shift($host);
    }
    $domain = implode('.', $host);
  }
  // In case a full host was given, extract the subdomain.
  $subdomain = explode('.', $subdomain);
  $subdomain = $subdomain[0];
  // Alow modules to alter it.
  drupal_alter('multisite_domain', $subdomain, $domain);
  return "$subdomain.$domain";
}

/**
 * Return a safe database name for a domain.
 */
function multisite_get_database_name($hostname) {
  return db_escape_field(preg_replace(array('/\./', '/\-/'), array('_', ''), $hostname));
}

/**
 * Reconnect to the database after reading in a settings.php file again.
 * @TODO we should use db_set_active() instead.
 */
function multisite_reconnect_database($hostname = NULL) {
  global $databases;
  if (is_string($hostname)) {
    $sites_path = DRUPAL_ROOT . "/sites/$hostname";
  }
  // Default to the current site.
  else {
    $sites_path = conf_path(TRUE, TRUE);
  }
  // Override the $databases array.
  include "$sites_path/settings.php";
  // Close the current connection and reparse the $databases array.
  db_close();
  Database::parseConnectionInfo();
}

/**
 * Return whether a specified table is shared.
 */
function _multisite_is_shared_table($table) {
  global $databases;
  return isset($databases['default']['default']['prefix'][$table]);
}

/**
 * Helper function to inspect all fields in the fields ui and make sure they 
 * are shared in the $database array.
 *
 * @see multisite_form_field_ui_field_overview_form_alter().
 * @see _multisite_verify_shared_table().
 * @see _multisite_is_shared_table().
 */
function _multisite_check_fields_ui($form, $form_state) {
  drupal_set_message(t('You should never reuse existing fields here! Instead you should create a new field, map it to the bundles existing field and add the tables to be shared in settings.php.'), 'notice');
  $form['#submit'][] = 'multisite_form_field_ui_field_overview_form_submit';
  foreach (element_children($form['fields']) as $field_name) {
    // Skip non fields.
    if (strpos($field_name, 'field_') !== 0) {
      continue;
    }
    $field_info = field_info_field($field_name);
    // Only SQL storage is supported for now
    if ($field_info['storage']['type'] != 'field_sql_storage') {
      drupal_set_message(t('The field @field is not using field_sql_storage as is required.', array('@field' => $field_name)), 'error');
    }
    foreach ($field_info['storage']['details']['sql'] as $key => $storage_detail) {
      $table = key($storage_detail);
      _multisite_verify_shared_table($field_name, $table);
    }
    switch ($field_info['type']) {
      case 'taxonomy_term_reference':
        _multisite_verify_shared_table($field_name, 'taxonomy_term_data');
        _multisite_verify_shared_table($field_name, 'taxonomy_term_hierarchy');
        _multisite_verify_shared_table($field_name, 'taxonomy_vocabulary');
        drupal_set_message(t('Make sure fields associated with the taxonomy terms are exported through features and used on all sites'), 'notice');

        break;
    }
  }
}

/**
 * Callback to notify the user the new multisite fields need to be configured
 * in settings.php.
 */
function multisite_form_field_ui_field_overview_form_submit($form, $form_state) {
  drupal_set_message(t('If you are creating a new field, remeber to configure its tables to be shared in settings.php'));
}

/**
 * Helper function to notify the user if a fields table isnt shared in the 
 * $databases array or if there might be a typo in its prefix.
 *
 * @see _multisite_is_shared_table().
 * @see _multisite_check_fields_ui().
 */
function _multisite_verify_shared_table($field_name, $table) {
  global $databases;
  $success = TRUE;
  // Make sure the fields tables have a prefixed entry.
  if (!_multisite_is_shared_table($table)) {
    drupal_set_message(t('The field %field is not configured to use a shared %table table in settings.php', array('%field' => $field_name, '%table' => $table)), 'error');
    $success = FALSE;
  }
  // Notify the user if the prefixed value is different from the database
  // (fields are normally only configured on the master site).
  elseif ($databases['default']['default']['prefix'][$table] != $databases['default']['default']['database'] . '.') {
    drupal_set_message(t('The field %field\'s %table table uses an unexpected prefix, make sure it\'s correct.', array('%field' => $field_name, '%table' => $table)), 'notice');
  }
  return $success;
}
