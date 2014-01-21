<?php
/**
 * @file
 * Returns the HTML for a single Drupal page.
 *
 * Complete documentation for this file is available online.
 * @see https://drupal.org/node/1728148
 */
?>

<div id="topnav">
  <nav role="navigation" tabindex="-2">
    <?php print render($page['topnav']); ?>
  </nav>
</div>

<div id="page">

  <header class="header" id="header" role="banner">

    <?php print render($page['header']); ?>

  </header>

  <div id="main">

    <div id="content" class="column" role="main">
      <div class="container">
        <?php print render($page['highlighted']); ?>
        <?php // print $breadcrumb; ?>
        <a id="main-content"></a>
        <?php print $messages; ?>
        <?php print render($tabs); ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?>
          <ul class="action-links"><?php print render($action_links); ?></ul>
        <?php endif; ?>
        <?php  print render($page['content']); ?>
        <?php // print $feed_icons; ?>
      </div>
    </div>

  </div>

</div>

  <div id="footer-wrapper">
    <?php print render($page['footer']); ?>
  </div>

<?php print render($page['bottom']); ?>
