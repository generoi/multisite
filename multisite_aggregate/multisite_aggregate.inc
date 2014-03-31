<?php

/**
 * MultisiteAggregate Entity
 */
class MultisiteAggregate extends Entity {
  public function __construct($values = array()) {
    parent::__construct($values, 'multisite_aggregate');
  }
  protected function defaultLabel() {
    return $this->title;
  }

  /**
   * Return the path to the source enity.
   */
  protected function defaultUri() {
    global $base_url;
    $multisite = multisite_load($this->msid);
    $scheme = parse_url($base_url, PHP_URL_SCHEME);
    return array(
      'path' => "$scheme://$multisite->hostname/node/$this->entity_id",
      'options' => array('external' => TRUE),
    );
  }
}

/**
 * MultisiteAggregateType Entity
 */
class MultisiteAggregateType extends Entity {
  public $type;
  public $label;

  public function __construct($values = array()) {
    parent::__construct($values, 'multisite_aggregate_type');
  }
}

class MultisiteAggregateController extends EntityAPIController {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }
  public function create(array $values = array()) {
    $values += array(
      'maid' => '',
      'entity_id' => '',
      'uuid' => '',
      'is_new' => TRUE,
      'title' => '',
      'created' => '',
      'changed' => '',
      'uid' => '',
      'msid' => '',
    );
    $aggregate = parent::create($values);
    return $aggregate;
  }
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
    return $content;
  }
}

class MultisiteAggregateUIController extends EntityDefaultUIController {
  public function hook_menu() {
    $items = parent::hook_menu();
    // Remove some admin paths which arent used, everything except list and
    // delete.
    unset($items[$this->path . '/add']);
    unset($items[$this->path . '/manage/%multisite_aggregate']);
    unset($items[$this->path . '/manage/%multisite_aggregate/edit']);
    unset($items[$this->path . '/manage/%multisite_aggregate/clone']);
    return $items;
  }
}

class MultisiteAggregateTypeController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function create(array $values = array()) {
    $values += array(
      'id' => '',
      'is_new' => TRUE,
      'source_type' => '',
      'source_bundle' => '',
      'source_fields' => '',
    );
    $aggregate_type = parent::create($values);
    return $aggregate_type;
  }
}

class MultisiteAggregateTypeUIController extends EntityDefaultUIController {
}

