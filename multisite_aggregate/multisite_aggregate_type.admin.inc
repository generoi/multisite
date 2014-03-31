<?php

/**
 * Form callback for configuring multisite aggregate types.
 */
function multisite_aggregate_type_form($form, &$form_state, $aggregate_type, $op = 'edit') {
  if ($op == 'clone') {
    $aggregate_type->label .= ' (cloned)';
    $aggregate_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $aggregate_type->label,
    '#description' => t('The human-readable name of this multisite aggregate type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($aggregate_type->type) ? $aggregate_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'multisite_aggregate_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this multisite aggregate type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['source'] = array(
    '#type' => 'fieldset',
    '#title' => t('Source'),
  );
  // Only nodes are allowed for now
  $form['source']['source_type'] = array(
    '#type' => 'hidden',
    '#value' => 'node',
  );
  $form['source']['source_bundle'] = array(
    '#type' => 'select',
    '#title' => t('Node bundle'),
    '#options' => node_type_get_names(),
    '#default_value' => !empty($aggregate_type->source_bundle),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save aggregate type'),
    '#weight' => 40,
  );
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function multisite_aggregate_type_form_submit(&$form, &$form_state) {
  $aggregate_type = entity_ui_form_submit_build_entity($form, $form_state);
  $aggregate_type->save();
  $form_state['redirect'] = 'admin/structure/multisite_aggregate_types';
}

/**
 * Form API submit callback for delete button.
 */
function multisite_aggregate_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/multisite_aggregate_types/manage/' . $form_state['multisite_aggregate_type']->type . '/delete';
}
