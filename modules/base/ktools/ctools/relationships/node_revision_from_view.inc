<?php

/**
 * @file
 * Plugin to provide an relationship handler for node revision from view.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Node revision from view'),
  'keyword' => 'node',
  'description' => t('Extract a node revision context from a view context of the base type node.'),
  'required context' => new ctools_context_required(t('View'), 'view', array('base' => 'node')),
  'context' => 'ktools_node_revision_from_view_context',
  'edit form' => 'ktools_node_revision_from_view_settings_form',
  'edit form validate' => 'ktools_node_revision_from_view_settings_form_validate',
  'defaults' => array('row' => 1),
);

/**
 * Return a new context based on an existing context.
 */
function ktools_node_revision_from_view_context($context, $conf, $placeholder = FALSE) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data) || $placeholder) {
    return ctools_context_create_empty('node', NULL);
  }
  $view = views_content_context_get_view($context);
  // Ensure the view executes, but we don't need its output.
  views_content_context_get_output($context);

  $row = intval($conf['row']) - 1;
  if (isset($view->result[$row])) {
    $nid = $view->result[$row]->node_revision_nid;
    $vid = $view->result[$row]->node_revision_vid;
    if ($nid && $vid) {
      $node = node_load($nid, $vid);
      return ctools_context_create('node', $node);
    }
  }
  return ctools_context_create_empty('node', NULL);
}

/**
 * Settings form for the relationship.
 */
function ktools_node_revision_from_view_settings_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['row'] = array(
    '#title' => t('Row number'),
    '#type' => 'textfield',
    '#default_value' => $conf['row'],
  );

  return $form;
}

function ktools_node_revision_from_view_settings_form_validate($form, &$form_state) {
  if (intval($form_state['values']['row']) <= 0) {
    form_error($form['row'], t('Row number must be a positive integer value.'));
  }
}
