<?php

/**
 * @file
 * Handles counts of node views via Ajax with minimal bootstrap.
 */

/**
 * Root directory of Drupal installation.
 */
define('DRUPAL_ROOT', substr($_SERVER['SCRIPT_FILENAME'], 0, strpos($_SERVER['SCRIPT_FILENAME'], '/sites/')));
// Change the directory to the Drupal root.
chdir(DRUPAL_ROOT);

include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);
if (variable_get('statistics_count_content_views', 0) && variable_get('statistics_count_content_views_ajax', 0)) {
  $nid = $_POST['nid'];
  $maid = $_POST['maid'];
  if (is_numeric($maid)) {
    db_merge('multisite_statistics')
      ->key(array('maid' => $maid))
      ->fields(array(
        'daycount' => 1,
        'totalcount' => 1,
        'timestamp' => REQUEST_TIME,
      ))
      ->expression('daycount', 'daycount + 1')
      ->expression('totalcount', 'totalcount + 1')
      ->execute();

  }
  if (is_numeric($nid)) {
    // Run the statistics own logic as well.
   db_merge('node_counter')
      ->key(array('nid' => $nid))
      ->fields(array(
        'daycount' => 1,
        'totalcount' => 1,
        'timestamp' => REQUEST_TIME,
      ))
      ->expression('daycount', 'daycount + 1')
      ->expression('totalcount', 'totalcount + 1')
      ->execute();
  }
}
