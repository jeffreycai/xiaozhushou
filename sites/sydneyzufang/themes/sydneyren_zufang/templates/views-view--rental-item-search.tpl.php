<?php

/**
 * @file
 * Main view template.
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */
?>




<div class="<?php print $classes; ?>">
  <?php if ($exposed): ?>
    <div class="view-filters">
      <?php print $exposed; ?>
    </div>
  <?php endif; ?>

  <?php print render($title_prefix); ?>
  <?php if ($title): ?>
    <?php print $title; ?>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <div id="search-breadcrumb" style="clear: both;">
    <?php $keyword = _get_request_var('search_api_views_fulltext');  ?>
    <?php $tid = _get_request_var('field_rental_suburb'); $term = taxonomy_term_load($tid); $suburb = $term->name; ?>
    <?php $property_type = _get_request_var('field_rental_property_type'); ?>
    <?php $rental_type = _get_request_var('field_rental_rental_type'); ?>
    <?php $suburbs = Utility::getInstance()->loadSettings('sydneytoday->suburbs'); ?>
    当前您的搜索为: 
      <?php if ($keyword): ?>
        <?php if (!in_array(ucfirst(strtolower($keyword)), $suburbs)): ?>
          基于关键字"<span><?php echo $keyword; ?></span>", 
        <?php else: ?>
          位于"<span><?php echo $keyword ?></span>"的
        <?php endif; ?>
      <?php elseif ($suburb): ?>
        位于"<span><?php echo $suburb ?></span>"的
      <?php endif; ?>
      
      <?php if ($rental_type): ?>
        "<span><?php echo $rental_type ?></span>"的
      <?php endif; ?>
        
      <?php if ($property_type): ?>
        "<span><?php echo $property_type; ?></span>"
      <?php endif; ?>
  </div>

  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>

  <?php if ($rows): ?>
    <div class="view-content">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
  <?php endif; ?>

  <?php if ($attachment_after): ?>
    <div class="attachment attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div><?php /* class view */ ?>
