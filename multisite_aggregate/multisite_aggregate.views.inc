<?php

/**
 * @file multisite_views.views.inc
 * TODO: Enter file description here.
 */

/**
 * Implements hook_views_data().
 */
function multisite_aggregate_views_data() {
  // Define the base group of this table.
  $data['multisite_aggregate']['table']['group'] = t('Multisite aggregates');

  // Advertise this table as a possible base table
  $data['multisite_aggregate']['table']['base'] = array(
    'field' => 'aid',
    'title' => t('Multisite aggregate'),
    'help' => t('Multisite aggregate'),
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

  $data['multisite_aggregate']['aid'] = $numeric_base + array(
    'title' => t('ID'),
    'help' => t('Unique identifier'),
  );
  $data['multisite_aggregate']['entity_id'] = $numeric_base + array(
    'title' => t('Entity ID'),
    'help' => t('Unique identifier for the aggregated entity'),
  );
  $data['multisite_aggregate']['type'] = $string_base + array(
    'title' => t('Entity Type'),
    'help' => t('Aggregated entity type'),
  );
  $data['multisite_aggregate']['bundle'] = $string_base + array(
    'title' => t('Entity Bundle'),
    'help' => t('Aggregated entity bundle'),
  );
  $data['multisite_aggregate']['uid'] = $numeric_base + array(
    'title' => t('Entity author ID'),
    'help' => t('Unique identifier for the aggregated entities author'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('The entities author'),
    ),
  );
  $data['multisite_aggregate']['label'] = $string_base + array(
    'title' => t('Entity Label'),
    'help' => t('Label of the aggregated entity'),
  );
  $data['multisite_aggregate']['language'] = $string_base + array(
    'title' => t('Entity Language'),
    'help' => t('Language of the aggregated entity'),
  );
  $data['multisite_aggregate']['uri'] = $string_base + array(
    'title' => t('Entity URI'),
    'help' => t('URI of the aggregated entity'),
  );
  $data['multisite_aggregate']['summary'] = array(
    'title' => t('Entity Summary'),
    'help' => t('Summary of the aggregated entity'),
    'field' => array(
      'handler' => 'views_handler_field_markup',
      'format' => filter_default_format(),
    ),
  ) + $string_base;

  $data['multisite_aggregate']['image'] = $string_base + array(
    'title' => t('Entity Image'),
    'help' => t('Image of the aggregated entity'),
  );
  $data['multisite_aggregate']['created'] = $date_base + array(
    'title' => t('Created'),
    'help' => t('The time aggregate was created.'),
  );
  $data['multisite_aggregate']['changed'] = $date_base + array(
    'title' => t('Chaged'),
    'help' => t('The time aggregate was last changed.'),
  );
  $data['multisite_aggregate']['sid'] = $numeric_base + array(
    'title' => t('Multisite ID'),
    'help' => t('Unique identifier for the aggregates multisite instance'),
    'relationship' => array(
      'base' => 'multisite',
      'field' => 'sid',
      'handler' => 'views_handler_relationship',
      'label' => t('The entities multisite instance'),
    ),
  );

  // @TODO might get crodd db joins to work for node properties, probably not 
  // for fields though as views does entity_load which we cant fool.
  // $data['multisite_aggregate']['node'] = array(
  //   'title' => t('Aggregated entity (node)'),
  //   'help' => t('The original node entity which is aggregated.'),
  //   'real field' => 'entity_id',
  //   'argument' => array(
  //     'field' => 'entity_id',
  //     'handler' => 'views_handler_argument_node_nid',
  //   ),
  //   'relationship' => array(
  //     'base' => 'node',
  //     'base field' => 'nid',
  //     'handler' => 'multisite_aggregate_views_handler_relationship',
  //     'label' => t('entity (node)'),
  //   ),
  // );
  // //
  // // For other base tables, explain how we join
  // $data['multisite_aggregate']['table']['join'] = array(
  //   // Directly links to node table.
  //   'node' => array(
  //     'left_field' => 'entity_id',
  //     'field' => 'nid',
  //   ),
  // );

  return $data;
}
