<?php

/**
 * This script is the AJAX service endpoint of the FontMetricWebServerService.
 * There is no use in calling this script directly, neither by a web server nor through cli.
 */

use Utils\Autoload\Autoloader;
use Utils\Graphics2D\Shapes\Styles\TextStyle;
use Utils\Font\FontManager;
use Utils\Font\FontMetric;

require_once __DIR__.'/../app/bootstrap.inc';

$result = '';

if(php_sapi_name() !== 'cli')
{
    if(array_key_exists('fontDefinitions', $_GET))
    {
        $textStyle = new TextStyle($_GET['fontFamily'], $_GET['fontSize']);

        $fontManager = new FontManager(null);

        $fontManager->addFontMetric(new FontMetric($textStyle, explode(',', $_GET['fontDefinitions'])));

        $result = $textStyle.' added successfully!';
    }
    else
        $result = 'no font defintions given!';
}
else
    $result = 'please run this script from a web browser!';

echo '"'.$result.'"';
