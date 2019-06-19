<?php
//include "/Users/harvinder/sites/gitgub_projects_harvyde/image/vendor/autoload.php";
include "/Users/harvinder/sites/gitgub_projects_harvyde/image/examples/ZImage.inc";

use PHPUnit\Framework\TestCase;
//use harvyde\example\ZImage;

ini_set('error_reporting', ini_get('error_reporting') & ~E_USER_DEPRECATED);

class ImageTest extends TestCase {

  const IMAGE = '/image.png';
  const IMAGE2x = '/image@2x.png';

  public function testSetup() {
    $img = new ZImage(static::IMAGE, [
      'path'=>'/Users/harvinder/sites/gitgub_projects_harvyde/image/tests/images/',
      'url-prefix'=>'/images/'
    ]);
    $img->alt = 'Alt text';
    $img->title = 'Image title';
    $imgHtml = $img->imgResponsive();
//    print $imgHtml;
    $this->assertStringContainsString("<img src=", $imgHtml);
    $this->assertStringContainsString("/images/image.png", $imgHtml);
    $this->assertStringContainsString("/images/image@2x.png", $imgHtml);
    return $imgHtml;
  }

  /**
   * @depends testSetup
   */
  public function testRetina($imgHtml) {
    $this->assertStringContainsString(static::IMAGE2x, $imgHtml);
  }

  public function testNotRetina() {
    $img = new ZImage(static::IMAGE);
    $img->retina = FALSE;
    $imgHtml = $img->imgResponsive();
    $this->assertStringNotContainsString("/images/image@2x.png", $imgHtml);
  }

  /**
   * @depends testSetup
   */
  public function testWidth($imgHtml) {
    $img = new ZImage(static::IMAGE);
    $this->assertEquals($img->width, 570);
  }

  /**
   * @depends testSetup
   */
  public function testHeight($imgHtml) {
    $img = new ZImage(static::IMAGE);
    $this->assertEquals($img->height, 416);
  }

  public function testCustomWidth() {
    $img = new ZImage(static::IMAGE);
    $img->width = 300;
    $img->height = 100;
    $imgHtml = $img->imgResponsive();
    $this->assertStringContainsString('max-width: 300px', $imgHtml);
    $this->assertStringContainsString('padding-bottom: 33.333333333333%;', $imgHtml);

  }

  public function testAltText() {
    $img = new ZImage(static::IMAGE);
    $img->alt = 'Lorem ipsum dolor sit amet';
    $imgHtml = $img->imgResponsive();
    $this->assertStringContainsString($img->alt, $imgHtml);
  }

  public function testTitleText() {
    $img = new ZImage(static::IMAGE);
    $img->title = 'consectetur adipisicing elit.';
    $imgHtml = $img->imgResponsive();

    $this->assertStringContainsString($img->title, $imgHtml);
  }

  public function testImgWrapper() {
    $img = new ZImage(static::IMAGE);
    $imgHtml = $img->img();
    $imgResponsiveHtml = $img->imgResponsive();
    $imgResponsiveHtml = str_replace('width: 100%; position: absolute; ', '', $imgResponsiveHtml);
    $this->assertStringContainsString($imgHtml, $imgResponsiveHtml);
  }

  public function testClickToEnlarge() {
    $img = new ZImage(static::IMAGE);
    $img->clickToEnlarge = TRUE;
    $imgHtml = $img->img();
    $this->assertStringContainsString('<a href="/images/image.png" class="fancybox-media d-block">', $imgHtml);
  }
}
