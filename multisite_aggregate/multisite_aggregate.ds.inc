<?php

/**
 * Implements hook_ds_fields_info().
 */
function multisite_aggregate_ds_fields_info($entity_type) {
  $fields['multisite_aggregate']['title'] = array(
    'title' => t('Title'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'ds_render_field',
    'properties' => array(
      'entity_render_key' => 'title',
      'settings' => array(
        'link' => array(
          'type' => 'select',
          'options' => array('no', 'yes'),
        ),
        'wrapper' => array(
          'type' => 'textfield',
          'description' => t('Eg: h1, h2, p'),
        ),
        'class' => array(
          'type' => 'textfield',
          'description' => t('Put a class on the wrapper. Eg: block-title'),
        ),
      ),
      'default' => array(
        'wrapper' => 'h2',
        'link' => 0,
        'class' => '',
      ),
    ),
  );

  // Entity link.
  $fields['multisite_aggregate']['entity_link'] = array(
    'title' => t('Read more'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'ds_render_field',
    'properties' => array(
      'settings' => array(
        'link text' => array('type' => 'textfield'),
        'wrapper' => array(
          'type' => 'textfield',
          'description' => t('Eg: h1, h2, p'),
        ),
        'class' => array(
          'type' => 'textfield',
          'description' => t('Put a class on the wrapper. Eg: block-title'),
        ),
      ),
      'default' => array(
        'link text' => 'Read more',
        'wrapper' => '',
        'class' => '',
        'link' => 1,
      ),
    ),
  );

   // Author.
  $fields['multisite_aggregate']['author'] = array(
    'title' => t('Author'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'ds_render_author_field',
    'properties' => array(
      'formatters' => array(
        'author' => t('Author'),
        'author_linked' => t('Author linked to profile'),
      ),
    ),
  );

  // Multisite.
  $fields['multisite_aggregate']['multisite'] = array(
    'title' => t('Multisite'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'multisite_ds_render_multisite_field',
    'properties' => array(
      'formatters' => array(
        'name' => t('Name'),
        'name_linked' => t('Name linked to site'),
      ),
    ),
  );

  // Created time.
  $format_types = system_get_date_types();
  $date_formatters = array();
  foreach ($format_types as $formatter) {
    $date_formatters['ds_post_date_' . $formatter['type']] = t($formatter['title']);
  }
  $fields['multisite_aggregate']['post_date'] = array(
    'title' => t('Post date'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'ds_render_date_field',
    'properties' => array(
      'formatters' => $date_formatters,
    ),
  );

  if (isset($fields[$entity_type])) {
    return array($entity_type => $fields[$entity_type]);
  }
}

/**
 * Render a multisite as a DS field.
 */
function multisite_ds_render_multisite_field($field) {
  if ($field['formatter'] == 'name') {
    return entity_label('multisite', $field['entity']->multisite);
  }
  if ($field['formatter'] == 'name_linked') {
    $uri = entity_uri('multisite', $field['entity']->multisite);
    $label = entity_label('multisite', $field['entity']->multisite);
    return l($label, $uri['path'], $uri['options']);
  }
}
