<?php

/**
 * Implements hook_views_default_views().
 */
function multisite_aggregate_views_default_views() {
  $view = new view();
  $view->name = 'multisite_aggregates_admin';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'multisite_aggregate';
  $view->human_name = 'Multisite Aggregates Admin';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'rendered_entity_1' => 'rendered_entity_1',
    'type' => 'type',
    'name_1' => 'name_1',
    'rendered_entity' => 'rendered_entity',
    'language' => 'language',
    'created' => 'created',
    'changed' => 'changed',
    'view_multisite_aggregate' => 'view_multisite_aggregate',
    'edit_multisite_aggregate' => 'view_multisite_aggregate',
    'delete_multisite_aggregate' => 'view_multisite_aggregate',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['info'] = array(
    'rendered_entity_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'language' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_multisite_aggregate' => array(
      'align' => '',
      'separator' => ' | ',
      'empty_column' => 0,
    ),
    'edit_multisite_aggregate' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'delete_multisite_aggregate' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Multisite Aggregate: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'multisite_aggregate';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Relationship: Multisite Aggregate: Multisite */
  $handler->display->display_options['relationships']['msid']['id'] = 'msid';
  $handler->display->display_options['relationships']['msid']['table'] = 'multisite_aggregate';
  $handler->display->display_options['relationships']['msid']['field'] = 'msid';
  /* Field: Multisite Aggregate: Rendered Multisite Aggregate */
  $handler->display->display_options['fields']['rendered_entity_1']['id'] = 'rendered_entity_1';
  $handler->display->display_options['fields']['rendered_entity_1']['table'] = 'views_entity_multisite_aggregate';
  $handler->display->display_options['fields']['rendered_entity_1']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity_1']['label'] = 'Title';
  $handler->display->display_options['fields']['rendered_entity_1']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity_1']['view_mode'] = 'token';
  $handler->display->display_options['fields']['rendered_entity_1']['bypass_access'] = 0;
  /* Field: Multisite Aggregate: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'multisite_aggregate';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name_1']['label'] = 'Author';
  /* Field: Multisite: Rendered Multisite */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_multisite';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'msid';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Site name';
  $handler->display->display_options['fields']['rendered_entity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['view_mode'] = 'token';
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: Multisite Aggregate: Aggregated Entity Language */
  $handler->display->display_options['fields']['language']['id'] = 'language';
  $handler->display->display_options['fields']['language']['table'] = 'multisite_aggregate';
  $handler->display->display_options['fields']['language']['field'] = 'language';
  $handler->display->display_options['fields']['language']['label'] = 'Language';
  /* Field: Multisite Aggregate: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'multisite_aggregate';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  /* Field: Multisite Aggregate: Date updated */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'multisite_aggregate';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  /* Field: Multisite Aggregate: Link */
  $handler->display->display_options['fields']['view_multisite_aggregate']['id'] = 'view_multisite_aggregate';
  $handler->display->display_options['fields']['view_multisite_aggregate']['table'] = 'multisite_aggregate';
  $handler->display->display_options['fields']['view_multisite_aggregate']['field'] = 'view_multisite_aggregate';
  $handler->display->display_options['fields']['view_multisite_aggregate']['label'] = 'Operations';
  /* Field: Multisite Aggregate: Edit link */
  $handler->display->display_options['fields']['edit_multisite_aggregate']['id'] = 'edit_multisite_aggregate';
  $handler->display->display_options['fields']['edit_multisite_aggregate']['table'] = 'multisite_aggregate';
  $handler->display->display_options['fields']['edit_multisite_aggregate']['field'] = 'edit_multisite_aggregate';
  /* Field: Multisite Aggregate: Delete link */
  $handler->display->display_options['fields']['delete_multisite_aggregate']['id'] = 'delete_multisite_aggregate';
  $handler->display->display_options['fields']['delete_multisite_aggregate']['table'] = 'multisite_aggregate';
  $handler->display->display_options['fields']['delete_multisite_aggregate']['field'] = 'delete_multisite_aggregate';
  /* Sort criterion: Multisite Aggregate: Date updated */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'multisite_aggregate';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  $handler->display->display_options['sorts']['changed']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['changed']['expose']['label'] = 'Date updated';
  /* Sort criterion: Multisite: Multisite Hostname */
  $handler->display->display_options['sorts']['hostname']['id'] = 'hostname';
  $handler->display->display_options['sorts']['hostname']['table'] = 'multisite';
  $handler->display->display_options['sorts']['hostname']['field'] = 'hostname';
  $handler->display->display_options['sorts']['hostname']['relationship'] = 'msid';
  $handler->display->display_options['sorts']['hostname']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['hostname']['expose']['label'] = 'Multisite Hostname';
  /* Sort criterion: Multisite Aggregate: Date created */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'multisite_aggregate';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Multisite: Multisite Hostname */
  $handler->display->display_options['filters']['hostname']['id'] = 'hostname';
  $handler->display->display_options['filters']['hostname']['table'] = 'multisite';
  $handler->display->display_options['filters']['hostname']['field'] = 'hostname';
  $handler->display->display_options['filters']['hostname']['relationship'] = 'msid';
  $handler->display->display_options['filters']['hostname']['operator'] = 'contains';
  $handler->display->display_options['filters']['hostname']['exposed'] = TRUE;
  $handler->display->display_options['filters']['hostname']['expose']['operator_id'] = 'hostname_op';
  $handler->display->display_options['filters']['hostname']['expose']['label'] = 'Multisite Hostname';
  $handler->display->display_options['filters']['hostname']['expose']['operator'] = 'hostname_op';
  $handler->display->display_options['filters']['hostname']['expose']['identifier'] = 'hostname';
  $handler->display->display_options['filters']['hostname']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    210394380 => 0,
    30037204 => 0,
  );
  /* Filter criterion: Multisite Aggregate: Aggregated Entity Language */
  $handler->display->display_options['filters']['language']['id'] = 'language';
  $handler->display->display_options['filters']['language']['table'] = 'multisite_aggregate';
  $handler->display->display_options['filters']['language']['field'] = 'language';
  $handler->display->display_options['filters']['language']['exposed'] = TRUE;
  $handler->display->display_options['filters']['language']['expose']['operator_id'] = 'language_op';
  $handler->display->display_options['filters']['language']['expose']['label'] = 'Language';
  $handler->display->display_options['filters']['language']['expose']['operator'] = 'language_op';
  $handler->display->display_options['filters']['language']['expose']['identifier'] = 'language';
  $handler->display->display_options['filters']['language']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    210394380 => 0,
    30037204 => 0,
  );
  /* Filter criterion: Multisite Aggregate: Aggregated Entity Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'multisite_aggregate';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    210394380 => 0,
    30037204 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Author';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    210394380 => 0,
    30037204 => 0,
  );

  /* Display: System */
  $handler = $view->new_display('system', 'System', 'system_1');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['path'] = 'admin/content/multisite_aggregates';
  $translatables['multisite_aggregates_admin'] = array(
    t('Master'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('User'),
    t('Multisite'),
    t('Title'),
    t('Type'),
    t('Author'),
    t('Site name'),
    t('Language'),
    t('Date created'),
    t('Date updated'),
    t('Operations'),
    t('Edit link'),
    t('Delete link'),
    t('Multisite Hostname'),
    t('System'),
  );

  $views[$view->name] = $view;
  return $views;
}
