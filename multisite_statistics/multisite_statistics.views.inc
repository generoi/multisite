<?php

/**
 * Implements hook_views_data_alter().
 */
function multisite_statistics_views_data_alter(&$data) {
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

  $data['multisite_aggregate']['daycount'] = $numeric_base + array(
    'title' => t('Views today'),
    'help' => t('The total number of times the node has been viewed today.'),
  );
  $data['multisite_aggregate']['totalcount'] = $numeric_base + array(
    'title' => t('Total views'),
    'help' => t('The total number of times the node has been viewed.'),
  );
  $data['multisite_aggregate']['timestamp'] = $date_base + array(
    'title' => t('Most recent view'),
    'help' => t('The most recent time the node has been viewed.'),
  );
}
