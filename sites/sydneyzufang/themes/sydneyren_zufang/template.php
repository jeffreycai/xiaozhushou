<?php
/**
 * @file
 * Contains the theme's functions to manipulate Drupal's default markup.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728096
 */


/**
 * Override or insert variables into the maintenance page template.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("maintenance_page" in this case.)
 */
/* -- Delete this line if you want to use this function
function sydneyren_zufang_preprocess_maintenance_page(&$variables, $hook) {
  // When a variable is manipulated or added in preprocess_html or
  // preprocess_page, that same work is probably needed for the maintenance page
  // as well, so we can just re-use those functions to do that work here.
  sydneyren_zufang_preprocess_html($variables, $hook);
  sydneyren_zufang_preprocess_page($variables, $hook);
}
// */

/**
 * Override or insert variables into the html templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("html" in this case.)
 */
//function sydneyren_zufang_preprocess_html(&$variables, $hook) {
  // The body tag's classes are controlled by the $classes_array variable. To
  // remove a class from $classes_array, use array_diff().
  //$variables['classes_array'] = array_diff($variables['classes_array'], array('class-to-remove'));
//  echo "<pre>";
//  print_r($variables);
//  die("</pre>");
//}


/**
 * Override or insert variables into the page templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("page" in this case.)
 */
/* -- Delete this line if you want to use this function
function sydneyren_zufang_preprocess_page(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the node templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("node" in this case.)
 */
/* -- Delete this line if you want to use this function
function sydneyren_zufang_preprocess_node(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');

  // Optionally, run node-type-specific preprocess functions, like
  // sydneyren_zufang_preprocess_node_page() or sydneyren_zufang_preprocess_node_story().
  $function = __FUNCTION__ . '_' . $variables['node']->type;
  if (function_exists($function)) {
    $function($variables, $hook);
  }
}
// */

/**
 * Override or insert variables into the comment templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("comment" in this case.)
 */
/* -- Delete this line if you want to use this function
function sydneyren_zufang_preprocess_comment(&$variables, $hook) {
  $variables['sample_variable'] = t('Lorem ipsum.');
}
// */

/**
 * Override or insert variables into the region templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("region" in this case.)
 */
/* -- Delete this line if you want to use this function
function sydneyren_zufang_preprocess_region(&$variables, $hook) {
  // Don't use Zen's region--sidebar.tpl.php template for sidebars.
  //if (strpos($variables['region'], 'sidebar_') === 0) {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('region__sidebar'));
  //}
}
// */

/**
 * Override or insert variables into the block templates.
 *
 * @param $variables
 *   An array of variables to pass to the theme template.
 * @param $hook
 *   The name of the template being rendered ("block" in this case.)
 */
/* -- Delete this line if you want to use this function
function sydneyren_zufang_preprocess_block(&$variables, $hook) {
  // Add a count to all the blocks in the region.
  // $variables['classes_array'][] = 'count-' . $variables['block_id'];

  // By default, Zen will use the block--no-wrapper.tpl.php for the main
  // content. This optional bit of code undoes that:
  //if ($variables['block_html_id'] == 'block-system-main') {
  //  $variables['theme_hook_suggestions'] = array_diff($variables['theme_hook_suggestions'], array('block__no_wrapper'));
  //}
}
// */

// Add some cool text to the search block form
//function sydneyren_zufang_form_alter(&$form, &$form_state, $form_id) {
//  if ($form_id == 'search_block_form') {
//    // HTML5 placeholder attribute
//    $form['search_block_form']['#attributes']['placeholder'] = t('请输入地区名，或任意关键字');
//    
//    if (!empty($form_state['input']) && isset($form_state['input']['search_block_form'])) {
//      $term = $form_state['input']['search_block_form'];
//      if (in_array(ucfirst($term), Utility::getInstance()->getSetting("sydneytoday->suburbs"))) {
//        drupal_goto('suburbs/' . strtolower($term));
//      }
//    }
//  }
//}


/**
 * Implement THEME_field()
 * 
 * @param type $variables
 * @return string
 */
function sydneyren_zufang_field__field_rental_images__rental($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<div class="field-label"' . $variables['title_attributes'] . '>' . $variables['label'] . ':&nbsp;</div>';
  }

  // Render the items.
  $output .= '<div class="flexslider"><ul class="field-items slides"' . $variables['content_attributes'] . '>';
  foreach ($variables['items'] as $delta => $item) {
    $classes = 'field-item ' . ($delta % 2 ? 'odd' : 'even');
    $output .= '<li class="' . $classes . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul></div>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . '"' . $variables['attributes'] . '>' . $output . '</div>';

  return $output;
}

/**
 * Implement theme_breadcrumb()
 * 
 * @param type $variables
 */
function sydneyren_zufang_breadcrumb($variables) {
  $rtn = "<nav id='breadcrumb'>";
  
  // for "rental item" page, we set the "Go back" link
  if (preg_match('/\/rental\-item\/\d+/', $_SERVER['REQUEST_URI'])) {
    if (preg_match('/\/rental\-search/', $_SERVER['HTTP_REFERER'])) {
      $rtn .= "<a href='javascript:history.go(-1)'><i class='fa fa-angle-left'></i> 返回搜索</a>";
    } else {
      $rtn .= "<a href='/rental-search'><i class='fa fa-angle-left'></i> 返回搜索</a>";
    }
  // for other pages, we set breadcrumb
  } else {
    $rtn .= "<a href='/'><i class='fa fa-angle-left'></i> 返回首页</a>";
  }
  
  
  $rtn.= "</nav>";
  
  return $rtn;
}