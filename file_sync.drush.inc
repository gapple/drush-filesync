<?php

/**
 * @file
 * Provide a file-sync command to sync files directories between environments.
 */

/**
 * Implements hook_drush_command().
 */
function file_sync_drush_command() {
  $commands = array();

  $commands['file-sync'] = array(
    'description' => 'Sync files directories between sites',
    'aliases' => array('fsync'),
    'arguments' => array(
      'source' => 'Source alias',
      'destination' => 'Destination alias', // TODO make this optional and default to @self
    ),
    'options' => array(
      'only-public' => 'Only sync the public files directory',
      'only-private' => 'Only sync the private files directory',
    ),
    'required-arguments' => TRUE,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'examples' => array(
      'drush fsync @source @destination' => 'Sync both the public and private files directories',
      'drush fsync --only-public @source @destination' => 'Only sync the public files directory',
    ),
  );

  return $commands;
}

/**
 * File sync command callback.
 */
function drush_file_sync($source, $destination) {

  $only_public = drush_get_option('only-public', FALSE);
  $only_private = drush_get_option('only-private', FALSE);

  if ($only_public && $only_private) {
    drush_log(dt('Only one of --only-public or --only-private can be set'), 'error');
    return false;
  }

  // drush_core_rsync() loads all of the arguments and options originally
  // passed to drush, so using drush_unset_option('private') is ineffective and
  // we need to hack the context instead.
  $args = &drush_get_context('DRUSH_COMMAND_ARGS', array());
  $original_args = $args;

  // TODO load all options from command definition.
  foreach (array('only-public', 'only-private') as $option) {
    $arg_index = array_search('--' . $option, $args);
    if ($arg_index !== FALSE) {
      unset($args[$arg_index]);
    }
  }
  $args = array_values($args);

  $public_result = FALSE;
  if (!$only_private) {
    drush_log(dt('Syncing public files'), 'status');
    $public_result = _drush_file_sync_path_alias($source, $destination, '%files');
  }

  $private_result = FALSE;
  if (!$only_public) {
    drush_log(dt('Syncing private files'), 'status');
    $private_result = _drush_file_sync_path_alias($source, $destination, '%private', $only_private? 'error': 'warning');
  }

  // Restore the original arguments just to be safe.
  $args = $original_args;

  if (
    (!$only_private && !$public_result)
    ||
    ($only_private && !$private_result)
  ) {
    // If public transfer is attempted but fails, it's always an error.
    // If private transfer is attempted but fails, it's an error if only
    // transferring private files was attempted.
    drush_log(dt('Sync could not complete successfully'), 'error');
  }
  else if (!$only_public && !$only_private && !$private_result) {
    // A private file path may not be defined, so it's only a warning if the
    // private transfer could not be completed while attempting to transfer all
    // files.
    drush_log(dt('Sync partially completed'), 'warning');
  }
  else {
    drush_log(dt('Sync completed successfully'), 'success');
  }
}

/**
 * Helper function to sync a single path between two environments.
 *
 * @param $source
 * @param $destination
 * @param $path_alias
 * @param $error_level
 *   If sync fails, what error level should be used.
 * @return bool
 */
function _drush_file_sync_path_alias($source, $destination, $path_alias, $error_level = 'error') {

  $additional_options = array();

  // Check that the path alias can be resolved properly before starting the transfer.
  $source_path = $source . ':'. $path_alias;
  $source_alias_info = drush_sitealias_evaluate_path($source_path, $additional_options);
  if (empty($source_alias_info['path-aliases'][$path_alias])) {
    drush_log(dt('Could not evaluate source path !path.', array('!path' => $source_path)), $error_level);
    return FALSE;
  }

  $destination_path = $destination . ':' . $path_alias;
  $destination_alias_info = drush_sitealias_evaluate_path($destination_path, $additional_options);
  if (empty($destination_alias_info['path-aliases'][$path_alias])) {
    drush_log(dt('Could not evaluate destination path !path.', array('!path' => $destination_path)), $error_level);
    return FALSE;
  }

  drush_invoke('rsync', array($source_path, $destination_path));

  // drush_command() doesn't pass along the return value from rsync for
  // drush_invoke() to make use of, so it's necessary to check the error log.
  return drush_get_error() === DRUSH_SUCCESS;
}
