<?php

/**
 * @file multisite_views.views.inc
 * TODO: Enter file description here.
 */

/**
 * Implements hook_views_data_alter().
 */
function multisite_aggregate_views_data_alter(&$data) {
  $data['multisite_aggregate']['table']['join']['multisite'] = array(
    'left_field' => 'msid',
    'field' => 'msid',
  );

  $data['multisite_aggregate']['view_multisite_aggregate'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the multisite aggregate.'),
      'handler' => 'views_handler_field_multisite_aggregate_link',
    ),
  );

  $data['multisite_aggregate']['edit_multisite_aggregate'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the multisite aggregate on the source multisite.'),
      'handler' => 'views_handler_field_multisite_aggregate_link_edit',
    ),
  );

  $data['multisite_aggregate']['delete_multisite_aggregate'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the multisite aggregate (not the source).'),
      'handler' => 'views_handler_field_multisite_aggregate_link_delete',
    ),
  );
}
