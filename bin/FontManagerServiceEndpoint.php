<?php

/**
 * This script is the AJAX service endpoint of the FontMetricWebServerService.
 * There is no use in calling this script directly, neither by a web server nor through cli.
 */

use \ws\loewe\Utils\Graphics2D\Shapes\Styles\TextStyle;
use \ws\loewe\Utils\Font\FontManager;
use \ws\loewe\Utils\Font\FontMetric;

require $_SERVER['DOCUMENT_ROOT'].'/'.getApplicationFolder().'/vendor/autoload.php';

$result = '';

if(php_sapi_name() !== 'cli') {
    if(array_key_exists('fontDefinitions', $_GET)) {
        $textStyle    = new TextStyle($_GET['fontFamily'], $_GET['fontSize']);
        $fontManager  = new FontManager(null);

        $fontManager->addFontMetric(new FontMetric($textStyle, explode(',', $_GET['fontDefinitions'])));

        $result = $textStyle.' added successfully!';
    }
    else {
        $result = 'no font defintions given!';
    }
}
else {
    $result = 'please run this script from a web browser!';
}

echo '"'.$result.'"';


function getApplicationFolder() {
  $folder = null;
  if(isset($_SERVER['REQUEST_URI'])) {
    $folder = substr($_SERVER['REQUEST_URI'], 1, strpos($_SERVER['REQUEST_URI'], '/', 1) - 1);
  }

  return $folder;
}