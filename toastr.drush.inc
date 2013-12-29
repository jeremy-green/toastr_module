<?php
/**
 * @file
 *   drush integration for toastr
 */
/**
 * The toastr library URI.
 */
define('TOASTR_DOWNLOAD_URI', 'https://github.com/CodeSeven/toastr/archive/master.zip');
/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * See `drush topic docs-commands` for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function toastr_drush_command() {
  $items = array();
  // the key in the $items array is the name of the command.
  $items['toastr_library'] = array(
    'callback' => 'drush_toastr_library',
    'description' => dt("Downloads the toastr library."),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH, // No bootstrap.
    'arguments' => array(
      'path' => dt('Optional. The path to install the Toastr library. If omitted Drush will use the default location.'),
    ),
    'aliases' => array('toastrlibrary'),
  );
  return $items;
}
/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function toastr_drush_help($section) {
  switch ($section) {
    case 'drush:toastr-library':
      return dt("Downloads the toastr library from github.com, default location is sites/all/libraries.");
  }
}
/**
 * Command to download the Toastr library.
 */
function drush_toastr_library() {
  if (!drush_shell_exec('type unzip')) {
    return drush_set_error(dt('Missing dependency: unzip. Install it before using this command.'));
  }
  $args = func_get_args();
  if ($args[0]) {
    $path = $args[0];
  }
  else {
    $path = 'sites/all/libraries';
  }
  // Create the path if it does not exist.
  if (!is_dir($path)) {
    drush_op('mkdir', $path);
    drush_log(dt('Directory @path was created', array('@path' => $path)), 'notice');
  }
  // Set the directory to the download location.
  $olddir = getcwd();
  chdir($path);
  $zipfile = 'master.zip';
  $filename - 'toastr-master';
  $dirname = basename('TOASTR_DOWNLOAD_URI', '.zip');
  // Remove any existing Toastr library directory
  if (is_dir($dirname)) {
    drush_log(dt('An existing version of the toastr library was overwritten at @path', array('@path' => $path)), 'notice');
  }
  // Remove any existing Toastr zip archive
  if (is_file($zipfile)) {
    drush_op('unlink', $zipfile);
  }
  // Download the zip archive
  if (!drush_shell_exec('wget ' . TOASTR_DOWNLOAD_URI)) {
    drush_shell_exec('curl -O ' . TOASTR_DOWNLOAD_URI);
  }
  if (is_file($zipfile)) {
    // Decompress the zip archive
    drush_shell_exec('unzip -qq -o ' . $zipfile);
    // Remove the zip archive
    drush_op('unlink', $zipfile);
  }
  // Set working directory back to the previous working directory.
  chdir($olddir);
  if (is_dir($path . '/' . $dirname)) {
    drush_log(dt('Drush was unable to download the Toastr library to @path', array('@path' => $path)), 'error');
  }
  else {
    drush_log(dt('Toastr library has been downloaded to @path', array('@path' => $path)), 'success');
  }
}
