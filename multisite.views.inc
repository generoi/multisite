<?php

/**
 * @file multisite.views.inc
 * TODO: Enter file description here.
 */

/**
 * Implements hook_views_data().
 */
function multisite_views_data() {
  // Define the base group of this table.
  $data['multisite']['table']['group'] = t('Multisite');

  // Advertise this table as a possible base table
  $data['multisite']['table']['base'] = array(
    'field' => 'sid',
    'title' => t('Multisite'),
    'help' => t('Multisite'),
    'weight' => -10,
  );

  $numeric_base = array(
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $string_base = array(
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['multisite']['sid'] = $numeric_base + array(
    'title' => t('ID'),
    'help' => t('Unique identifier'),
  );
  $data['multisite']['name'] = $string_base + array(
    'title' => t('Multisite name'),
    'help' => t('The sitename variable of the multisite'),
  );
  $data['multisite']['host'] = $string_base + array(
    'title' => t('Hostname'),
    'help' => t('Hostname of the multisite'),
  );
  $data['multisite']['db'] = $string_base + array(
    'title' => t('Database'),
    'help' => t('The database name of the multisite'),
  );
  $data['multisite']['uid'] = $numeric_base + array(
    'title' => t('Multisite owner'),
    'help' => t('Unique identifier for the main user of the multisite'),
    'relationships' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('The multisites owner'),
    ),
  );
  return $data;
}
