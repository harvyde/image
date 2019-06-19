<?php
//namespace harvyde\example;
include "/Users/harvinder/sites/gitgub_projects_harvyde/image/vendor/autoload.php";

use harvyde\Image;

class ZImage extends Image {

  function __construct($image, $options = []) {
    $this->debug = 1;
    $_options = [
      'path' => '/Users/harvinder/sites/gitgub_projects_harvyde/image/tests/images/',
      'url-prefix' => '/images/'
    ];
    $options = array_merge($_options, $options);
    parent::__construct($image, $options);
  }
}