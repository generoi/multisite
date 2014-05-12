<?php

/**
 * Implements hook_ds_fields_info().
 */
function multisite_ds_fields_info($entity_type) {
  $fields['multisite']['title'] = array(
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
  $fields['multisite']['entity_link'] = array(
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
  $fields['multisite']['author'] = array(
    'title' => t('Author (owner)'),
    'field_type' => DS_FIELD_TYPE_FUNCTION,
    'function' => 'multisite_ds_render_author_field',
    'properties' => array(
      'formatters' => array(
        'author' => t('Author'),
        'author_linked' => t('Author linked to profile'),
      ),
    ),
  );

  if (isset($fields[$entity_type])) {
    return array($entity_type => $fields[$entity_type]);
  }
}

/**
 * DS field callback.
 */
function multisite_ds_render_author_field($field) {
  $user = user_load($field['entity']->uid);

  switch ($field['formatter']) {
    case 'author':
      return $user->name;
    case 'author_linked':
      return theme('username', array('account' => $user));
  }
}
