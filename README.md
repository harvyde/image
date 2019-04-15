a library for detecting sites / hosts and evironments. This library can be useful to build a multisite. Multiple domains pointing to the same application. You can use this class anywhere in your application to serve content based on domain and evironment site be being accessed.

# usage
```php
    $img = new Image('path/to/your/image.extension');
    #if you have ext-gd widht and height will be auto detected
    # $img->width = 300;
    # $img->height = 100;
    # if not set it will be detected from @2x image file exists. exists in th
    # $img->retina = false;
    
    print $img->imgResponsive();
```

#Support and Feedback
If you find a bug, please submit the issue in Github directly. [harvyde/image](https://github.com/harvyde/image/issues)  Issues



