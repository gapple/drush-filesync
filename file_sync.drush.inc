<?php

function file_sync_drush_command() {
  $commands = array();

  $commands['file-sync'] = array(
    'description' => 'Sync files directory between sites',
    'aliases' => array('fsync'),
    'arguments' => array(
      'source' => 'Source alias',
      'destination' => 'Destination alias',
    ),
    'options' => array(
      'private' => 'Sync private files',
    ),
    'required-arguments' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'examples' => array(
      'drush fsync @source @destination' => 'Sync public files directory from source to destination alias',
      'drush fsync --private @source @destination' => 'Sync private files directory from source to destination alias',
    ),
  );

  return $commands;
}

function drush_file_sync($source, $destination) {

  $additional_options = array();

  $path_alias = drush_get_option('private', FALSE)? '%private' : '%files';

  // Check that the path aliases can be resolved properly before starting the transfer.
  $source_alias_info = drush_sitealias_evaluate_path($source . ':'. $path_alias, $additional_options);
  if (empty($source_alias_info['path-aliases'][$path_alias])) {
    return drush_set_error('DRUSH_BAD_PATH', dt('Could not evaluate source path !path.', array('!path' => $source)));
  }

  $destination_alias_info = drush_sitealias_evaluate_path($destination . ':' . $path_alias, $additional_options);
  if (empty($destination_alias_info['path-aliases'][$path_alias])) {
    return drush_set_error('DRUSH_BAD_PATH', dt('Could not evaluate destination path !path.', array('!path' => $destination)));
  }

  drush_log('Starting rsync', 'ok');
  drush_unset_option('private');
  drush_invoke('rsync', array($source . ':' . $path_alias, $destination . ':' . $path_alias));
  drush_log('Sync complete', 'ok');
}
