<?php

/**
 * @file multisite.views.inc
 * Provide views integration for the multisite_process table and describe its
 * relationshpi to multisites. Additionally add extra handlers to multisite
 * that are not provided by entity_api yet.
 * @see https://drupal.org/node/1333870
 */

/**
 * Implements hook_views_data().
 */
function multisite_views_data() {
  // Define the base group of this table.
  $data['multisite_process']['table']['group'] = t('Multisite Process');

  // Advertise this table as a possible base table
  $data['multisite_process']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Multisite Process'),
    'help' => t('Multisite Process'),
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
  $date_base = array(
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['multisite_process']['id'] = $numeric_base + array(
    'title' => t('ID'),
    'help' => t('Unique identifier'),
  );
  $data['multisite_process']['msid'] = $numeric_base + array(
    'title' => t('Multisite ID'),
    'help' => t('Unique identifier for the multisite'),
    'relationships' => array(
      'base' => 'multisite',
      'field' => 'msid',
      'handler' => 'views_handler_relationship',
      'label' => t('multisite'),
    ),
  );
  $data['multisite_process']['created'] = $date_base + array(
    'title' => t('Created'),
    'help' => t('Date the installation process was initalized'),
  );
  $data['multisite_process']['completed'] = $date_base + array(
    'title' => t('Completed'),
    'help' => t('Date the installation process was completed'),
  );
  $data['multisite_process']['uid'] = $numeric_base + array(
    'title' => t('User'),
    'help' => t('Unique identifier for the user who initalized the installation process'),
    'relationships' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('user'),
    ),
  );

  $data['multisite_process']['table']['join'] = array(
    'multisite' => array(
      'left_field' => 'msid',
      'field' => 'msid',
    ),
  );
  $data['multisite_process']['table']['default_relationship'] = array(
    'multisite' => array(
      'table' => 'multisite',
      'field' => 'msid',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function multisite_views_data_alter(&$data) {
  $data['multisite']['table']['default_relationship'] = array(
    'multisite_process' => array(
      'table' => 'multisite_process',
      'field' => 'msid',
    ),
  );

  $data['multisite']['view_multisite'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the multisite.'),
      'handler' => 'views_handler_field_multisite_link',
    ),
  );

  $data['multisite']['edit_multisite'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the multisite.'),
      'handler' => 'views_handler_field_multisite_link_edit',
    ),
  );

  $data['multisite']['delete_multisite'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the multisite.'),
      'handler' => 'views_handler_field_multisite_link_delete',
    ),
  );
}
