<?php

/**
 * @file multisite_views.views.inc
 * TODO: Enter file description here.
 */

function multisite_views_views_plugins() {
  return array(
    'module' => 'multisite_views',
    'query' => array(
      'views_plugin_multisite_query' => array(
        'title' => t('Multisite Query'),
        'handler' => 'views_plugin_multisite_query',
        'parent' => 'views_plugin_query_default',
        'help' => t('Query which allows switching database dynamically.'),
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 *
 * Change the query class for all tables to allow multisite.
 */
function multisite_views_views_data_alter(&$data) {
  foreach ($data as $table => $info) {
    if (empty($info['table']['base'])) {
      continue;
    }
    if (!isset($info['table']['base']['query class']) || $info['table']['base']['query class'] == 'views_plugin_query_default') {
      $data[$table]['table']['base']['query class'] = 'views_plugin_multisite_query';
    }
  }
}

/**
 * Implements hook_views_pre_render().
 *
 * If a base url has been set, change it globally while the view is being 
 * rendered so that url() has the correct version.
 *
 * @see multisite_views_views_post_render().
 */
function multisite_views_views_pre_render(&$view) {
  global $base_url;
  if (!empty($view->query->options['base_url'])) {
    $view->original_base_url = $base_url;
    $base_url = $view->query->options['base_url'];
  }
}

/**
 * Implements hook_views_post_render().
 *
 * Set $base_url back to the original.
 *
 * @see multisite_views_views_pre_render().
 */
function multisite_views_views_post_render(&$view) {
  global $base_url;
  if (!empty($view->original_base_url)) {
    $base_url = $view->original_base_url;
  }
}
