<?php

/**
 * Stream wrapper for accessing a file across multisites.
 */
class MultisiteRootPublicStreamWrapper extends DrupalPublicStreamWrapper {
  public function getDirectoryPath() {
    return 'sites';
  }
}

/**
 * Get a stream uri for accessing a file across multisites.
 *
 * @param string $file_path
 *   The file path without the stream scheme.
 */
function _multisite_get_stream_path($file_path = '') {
  $conf_path = conf_path(FALSE, FALSE);
  // Remove the sites dir as it's included in the stream wrapper.
  $conf_path = explode('/', $conf_path);
  array_shift($conf_path);
  $conf_path = implode('/', $conf_path);
  return 'root://' . $conf_path . '/files/' . $file_path;
}

