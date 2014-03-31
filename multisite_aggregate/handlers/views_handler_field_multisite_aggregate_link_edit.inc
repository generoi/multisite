<?php

class views_handler_field_multisite_aggregate_link_edit extends views_handler_field_multisite_aggregate_link {
  function construct() {
    parent::construct();
  }

  function render_link($data, $values) {
    // ensure user has access to edit this multisite.
    if (!multisite_aggregate_access('update')) {
      return;
    }
    if (!parent::render_link($data, $values)) {
      return;
    }
    $this->options['alter']['path'] .= '/edit';
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    return $text;
  }
}
