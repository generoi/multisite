<?php

class views_handler_field_multisite_link_delete extends views_handler_field_multisite_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
  }

  function render_link($data, $values) {
    // ensure user has access to delete this multisite.
    $multisite = new stdClass();
    $multisite->msid = $this->get_value($values, 'msid');
    $multisite->uid = $this->get_value($values, 'uid');
    if (!multisite_access('delete', $multisite)) {
      return;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "admin/people/multisite/manage/$multisite->msid/delete";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    return $text;
  }
}
