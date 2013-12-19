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

    <?php if ($site_name || $site_slogan): ?>
      <div class="header__name-and-slogan" id="name-and-slogan">
        <i class="fa fa-home header__logo" id="logo"></i>
        <?php if ($site_name): ?>
          <h1 class="header__site-name" id="site-name">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" class="header__site-link" rel="home"><?php // print $site_name; ?><span class="highlight">悉尼</span><br />租房网</a>
          </h1>
        <?php endif; ?>
        <div class="clear"></div>
        <div id="site-address">www.<span class="highlight">sydneyzufang</span>.com</div>
        <?php if ($site_slogan): ?>
          <br />
          <div class="header__site-slogan" id="site-slogan"><?php print $site_slogan; ?></div>
          <div class="clear"></div>
        <?php endif; ?>
      </div>
    <?php endif; ?>

    <?php print render($page['header']); ?>

  </header>

  <div id="main">

    <div id="content" class="column" role="main">
      <?php print render($page['highlighted']); ?>
      <?php print $breadcrumb; ?>
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

  <?php print render($page['footer']); ?>

</div>

<?php print render($page['bottom']); ?>
