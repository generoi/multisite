<?php

class EntityReference_SelectionHandler_Multisite_Views extends EntityReference_SelectionHandler_Views {
  /**
   * Overrides EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new EntityReference_SelectionHandler_Multisite_Views($field, $instance);
  }
  public function getReferencableEntities($match = NULL, $match_operator = 'CONTAINS', $limit = 0) {
    $display_name = $this->field['settings']['handler_settings']['view']['display_name'];
    $args = $this->field['settings']['handler_settings']['view']['args'];
    $result = array();
    if ($this->initializeView($match, $match_operator, $limit)) {
      // Get the results.
      $result = $this->view->execute_display($display_name, $args);
    }

    $return = array();
    if ($result) {
      $target_type = $this->field['settings']['target_type'];
      $database = !empty($this->view->query->options['multisite']) ? $this->view->query->options['multisite'] : FALSE;
      if ($database) {
        db_set_active($database);
      }
      $entities = entity_load($target_type, array_keys($result));
      foreach($entities as $entity) {
        list($id,, $bundle) = entity_extract_ids($target_type, $entity);
        $return[$bundle][$id] = $result[$id];
      }
      if ($database) {
        db_set_active();
      }
    }
    return $return;
  }
}
