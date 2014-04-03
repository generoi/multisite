<?php

class views_plugin_multisite_query extends views_plugin_query_default {

  function options_definition() {
    global $base_url;
    $options = parent::options_definition();
    $options['multisite'] = array(
      'default' => 'default',
    );
    $options['base_url'] = array(
      'default' => $base_url,
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['multisite'] = array(
      '#type' => 'textfield',
      '#title' => t('Database'),
      '#description' => t('Set the database to use as configured in settings.php'),
      '#default_value' => $this->options['multisite'],
    );
    $form['base_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Base URL'),
      '#description' => t('Set the base url to use while rendering the view. This only works for rewritten links!'),
      '#default_value' => $this->options['base_url'],
    );
  }

  function query($get_count = FALSE) {
    if (!empty($this->options['multisite'])) {
      $this->view->base_database = $this->options['multisite'];
    }

    return parent::query($get_count);
  }
}
