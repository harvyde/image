<?php
namespace harvyde;

class Image {
  public $width;
  public $height;
  public $bits;
  public $mime;

  public $widthAdj;
  public $heightAdj;

  public $maxWidth = 0;

  protected $classes = [
    'imgParentClasses' => ['img-parent', 'clearfix'],
    'imgResponsiveClasses' => ['responsive-img-wrapper', 'clearfix'],
    'imgClasses' => [],
  ];

  public $imgParentClasses;
  public $imgResponsiveClasses;
  public $imgClasses;


  public $align;
  public $alt;
  public $title;
  public $retina;
  public $clickToEnlarge;

  /**
   * @var
   * img-parent is required to fix the unexpected height caused by padding-bottom
   */
  public $imgParentStyles;
  public $imgResponsiveStyles;
  public $imgStyles;

  public $imagesServerPath;
  public $imagesUrlPath;

  public $image;
  public $imageTwoX;

  public $src;
  public $srcOneX;
  public $srcTwoX;

  public $srcset;

  function __construct($image) {
    $this->debug = 0;
    $this->image = ltrim($image, "/");
    $this->setImagesPath();
    @$this->setImageDimensions();
    @$this->setIsImageRetina();
    $this->clickToEnlarge = false;
    $this->largeImage = false;
  }

  /**
   * @param Set image relative path
   * todo
   */
  function setImagesPath() {
    $this->imagesServerPath = "/Users/harvinder/sites/image/tests/images/";
    $this->imagesUrlPath = "/images/";
  }

  /**
   * @param Set image width and height by using relative path of image
   */
  function setImageDimensions() {
    if (file_exists($this->imagesServerPath . $this->image)) {
      $size = getimagesize($this->imagesServerPath . $this->image);
      $this->width = $size[0];
      $this->height = $size[1];
      $this->bits = $size['bits'];
      $this->mime = $size['mime'];
    }
  }

  function setIsImageRetina() {
    $this->imageTwoX = preg_replace("/(.*)\\.(jpeg|jpg|png|gif)/", "$1@2x.$2", $this->image);

    if (file_exists($this->imagesServerPath . $this->imageTwoX)) {
      $this->retina = TRUE;
    }
  }

  /**
   * @param string $element imgParentClasses, imgResponsiveClasses, imgClasses
   * @param mixed $newClass
   */
  function addClass($element, $newClass) {
    if (isset($element) && array_key_exists($element, $this->classes)) {
      if (is_string($newClass)) {
        $class[] = $newClass;
      } else {
        $class = $newClass;
      }
      $this->classes[$element] = array_merge($this->classes[$element], $class);
    } else {
      /**
       * trigger error on local env only
       */
    }
  }

  function getClasses($element = null) {
    if (isset($this->classes[$element]) && !empty($this->classes[$element])) {
      return implode(" ", $this->classes[$element]);
    }
    return null;
  }

  function getStyles($property = 'imgResponsiveStyles') {
    $s = "";
    if (!empty($this->{$property})) {
      foreach ($this->{$property} as $k => $v) {
        $s .= $k . ": " . $v . "; ";
      }
    }
    return $s;
  }


  function pr($a = null) {
    if ($this->debug) {
      print('<pre>');
      echo htmlentities($a);
      print('</pre>');
    }

  }

  /**
   * set image absolute urls, cdn logic works here
   */
  function imgUrls() {
    $this->src = $this->srcOneX = $this->imagesUrlPath . $this->image;
    if ($this->retina) {
      /**
       * it is important to use absolute paths for 2x images as well for cdn to work
       */
      $this->srcTwoX = $this->imagesUrlPath . $this->imageTwoX;
    }

    if ($this->largeImage) {
      /**
       * it is important to use absolute paths for 2x images as well for cdn to work
       */
      $this->largeImage = $this->imagesUrlPath . $this->largeImage;
    }


  }

  function srcset() {
    $srcset = "{$this->src} 1x";
    $this->isRetinaExists();
    if ($this->retina) {
      $srcset .= ', ' . $this->srcTwoX . ' 2x';
    }
    $this->srcset = $srcset;
  }

  /**
   * disable retina if 2x image not exists
   */
  function isRetinaExists() {
    if (file_exists($this->imagesServerPath . $this->imageTwoX)) {
//      image exists go ahead
    } else {
      $this->retina = FALSE;
      /**
       * comment watchdog messages for now, as it logging to many messages
       */
//      watchdog('Zimage', '2x image is missing, fallback to 1x @file  ', ['@file' => $this->imagesServerPath.$this->imageTwoX], WATCHDOG_NOTICE);
    }
  }


  function img() {
    /**
     * todo move imgUrls to constructor
     */
    $this->imgUrls();
    $this->srcset();


    $this->imgStyles['max-width'] = "100%";
    $styles = $this->getStyles('imgStyles');
    $img = <<<EOF
<img src="{$this->src}" srcset="{$this->srcset}" alt="$this->alt" title="$this->title" class="zzzz-cdn-changes-zzzz {$this->getClasses('imgClasses')}" style="$styles">
EOF;
    $largeImage = ($this->largeImage)?$this->largeImage:$this->src;
    if ($this->clickToEnlarge) {
      $img = <<<EOF
    <a href="{$largeImage}" class="fancybox-media d-block"> $img </a>
EOF;
    }
    return $img;
  }

  function imgResponsive() {
    $this->widthAdj = $this->width;
    $this->heightAdj = $this->height;
    if ($this->maxWidth && $this->maxWidth < $this->width) {
      $this->widthAdj = $this->maxWidth;
      $this->heightAdj = $this->height * $this->maxWidth / $this->width;
    }

    $height_percent = $this->heightAdj / $this->widthAdj * 100;
    $this->imgResponsiveStyles['padding-bottom'] = $height_percent . "%";

    $this->imgParentStyles['max-width'] = "{$this->widthAdj}px";
    $this->imgResponsiveStyles['max-width'] = "{$this->widthAdj}px";
    $this->imgResponsiveStyles['position'] = "relative";

    $this->imgStyles['width'] = "100%";
    $this->imgStyles['position'] = "absolute";

    $imgParentStyles = $this->getStyles('imgParentStyles');
    $imgResponsiveStyles = $this->getStyles('imgResponsiveStyles');
    $img = $this->img();

    $html = <<<EOF
<div class="{$this->getClasses('imgParentClasses')}" style="$imgParentStyles">
  <div class="{$this->getClasses('imgResponsiveClasses')}" style="$imgResponsiveStyles">
    $img
  </div>
</div>
EOF;

    return $html;
  }

}


?>