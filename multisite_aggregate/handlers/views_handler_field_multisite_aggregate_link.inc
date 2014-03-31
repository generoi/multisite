<?php

class views_handler_field_multisite_aggregate_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['maid'] = 'maid';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array(
      'default' => '',
      'translatable' => TRUE,
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );

    // The path is set by render_link function so don't allow to set it.
    $form['alter']['path'] = array('#access' => FALSE);
    $form['alter']['external'] = array('#access' => FALSE);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $value = $this->get_value($values, 'maid');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  function render_link($data, $values) {
    if (!($aggregate = multisite_aggregate_load($data))) {
      return;
    }
    $uri = $aggregate->uri();
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $uri['path'];
    $this->options['alter']['options'] = $uri['options'];
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    return $text;
  }
}
