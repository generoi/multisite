<?php

class views_handler_field_multisite_aggregate_link_delete extends views_handler_field_multisite_aggregate_link {
  function render_link($data, $values) {
    // ensure user has access to delete this multisite aggregate.
    if (!multisite_aggregate_access('delete')) {
      return;
    }
    $maid = $this->get_value($values, 'maid');

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "admin/content/multisite_aggregates/manage/$maid/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    return $text;
  }
}
