<?php

/**
 * Implements of hook_drush_help().
 */
function session_drush_help($section) {
  switch ($section)  {
    case 'drush:session-delete':
      return dt('Delete user sessions from the database.');
  }
}

/**
 * Implements of hook_drush_command().
 */
function session_drush_command() {
  $items['session-delete'] = array(
    'description' => 'Delete user sessions from the database.',
    'examples' => array(
      'drush session-delete all' => 'Delete all sessions.',
      'drush session-delete 64' => 'Delete sessions for user id 64.',
    ),
    'aliases' => array('sess-del'),
  );
  return $items;
}

/**
 * Command callback.
 *
 * @param $arg
 *   The user id to delete or 'all'.
 */
function drush_core_session_delete($arg = NULL) {
  if ($arg == 'all') {
    drush_print(dt('All sessions will be deleted.'));
    if (!drush_confirm(dt('Do you really want to continue?'))) {
      return drush_user_abort();
    }
    drush_db_delete('sessions');
    drush_log(dt('All sessions have been deleted.'), 'ok');
  }
  else if (is_numeric($arg)) {
    drush_print(dt('Sessions for uid !uid will be deleted.', array('!uid' => $arg)));
    if(!drush_confirm(dt('Do you really want to continue?'))) {
      return drush_user_abort();
    }
    $affected_rows = drush_db_delete('sessions', 'uid=:uid', array(':uid' => $arg));
    if ($affected_rows >= 1) {
      drush_log(dt('All sessions for uid !uid have been deleted.', array('!uid' => $arg)), 'ok');
    }
    else {
      return drush_set_error(dt('No sessions for uid !uid exist.', array('!uid' => $arg)));
    }
  }
  else {
    return drush_set_error(dt('No arguments provided.'));
  }
}
