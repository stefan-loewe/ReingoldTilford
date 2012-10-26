<?php

use \ws\loewe\Utils\Tree\TreeNode;
use \ws\loewe\Utils\Tree\DotTransformer;
use \ws\loewe\ReingoldTilford\Models\TreeNodeTreeModel;
use \ws\loewe\ReingoldTilford\Algorithms\ReingoldTilfordAlgorithm;
use \ws\loewe\ReingoldTilford\Plotters\Plotter;
use \ws\loewe\ReingoldTilford\Styles\TreeStyle;
use \ws\loewe\Utils\Graphics2D\Shapes\Styles\TextStyle;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\ImageDrawingPane;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\GdLibDrawingPane;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\ImagickDrawingPane;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\HtmlDrawingPane;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\HtmlMapDrawingPane;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\SvgDrawingPane;
use \ws\loewe\Utils\Geom\Point;
use \ws\loewe\Utils\File\FileSystemObject;
use \ws\loewe\Utils\File\Directory;
use \ws\loewe\Utils\Font\FontManager;
use \ws\loewe\Utils\Font\FontMetricServices\FontMetricServiceFactory;
use \ws\loewe\Utils\URL\Url;
use \ws\loewe\Utils\Logging\Logger;

require __DIR__.'/../vendor/autoload.php';

/*
 * TODO: keine verschiedenen Panes explizit erzeugen, sondern eine allgemeine, und dort Primitive hinzufügen, dann pane->save(HTML)->save(PNG);
 *  entweder comands sammeln und bei save einfach replay, oder Primitve mit Position, Border etc. speichern, und dann erzeugen (erstes leicht, zweites sauber)
 * TODO: consider decorator for Styles/Shapes ... maybe styles are obsolete as now we have shapes
 * TODO: add export to HtmlCanvas, (TC)Pdf
 * TODO: add support for units (HTML only?)
 *
 * TODO: iterative-recursiv bei XML etc --> push() -> Tag öffnen, pop() -> Tag schliesen -> auch für nested sets
 * TODO: reimplemtn nested-export
 * TODO: canvas breiter als 32768 -> split .. was schlaues überlegen .. evtl. zoom/scale auf canvas ..? 1. test gescheitert see https://developer.mozilla.org/en/Canvas_tutorial/Transformations
 * TODO: satelliteView als echte View, d.h. identisches image/HTML-Fragment, insb. interesant bei Updates im Baum -> nur eine Ressource zu ändern! -> nur ein png für Image und Statellite geht nicht (gut), weil edges zu schmal
 * TODO: Opera scrollen geht nicht mehr wenn thumbnail breiter fensterbreite (in scrollBy() frame.scrollLeft immer gleich 0, auch wenn explizit auf konstanten Wert gesetzt [Opera Bug!])
 *
 * DONE: maybe quit all of the document-specific moving/resizing, and try to get same images on different docs with other styles, only
 *          - exceptional handling still needed for HTML and GdLib, so no use, and a ShapeStyleConverter would be thus useless
 *          - counter-intuitive to have one style resulting in different exports
 * DONE: consider decorator for Shapes
 *          - current setters (setStrokeWidth, setStrokeColor etc) are easy enough, no need to introduce more noise
*/

Logger::setLogLevel(Logger::OFF);

ini_set('memory_limit', '4294967296');

set_time_limit(0);

error_reporting(E_ALL | E_STRICT);


$type = 'FileSystem';
//$type = 'JSON';
//$type = 'DOT';
$type = '';

//write example with minimal dependecies!

$root = getRoot($type);

$start = microtime(TRUE);

$algorithm = new ReingoldTilfordAlgorithm();

if($root instanceof FileSystemObject)
    $model = new FSTreemodel($root);

else
    $model = new TreeNodeTreeModel($root);

$startLayout = microtime(true);
$layout = $algorithm->getLayout($model, TreeStyle::CENTERED)->normalize();
echo "\n".'gotLayout in: '.(microtime(true) - $startLayout);

$style = new TreeStyle();

$strings = getStrings($root, $type);

//$metric = getFontMetric($style->textStyle);

//extract modules für PHPLint ..?

//$style->shapeStyle->setWidth($metric->getMaxWidth($strings));
//$style->shapeStyle->setHeight($metric->getMaxHeight($strings));


$startPlot = microtime(true);
$dimension = $layout->getDimension($style);
$plotter = new Plotter($model, $layout, null, null, null);
echo "\n".'gotPlot in: '.(microtime(true) - $startPlot);


$doExport = array();
$doExport['GD']       = 0;
$doExport['GIF']      = 0;
$doExport['JPG']      = 0;
$doExport['PNG']      = 0;
$doExport['IMAGICK']  = 0;
$doExport['SVG']      = 1;
$doExport['HTML']     = 0;
$doExport['MAP']      = 0;

if($doExport['GD'])
{
    if($doExport['GIF'])
    {
        $export = $plotter->plot($style, new GdLibDrawingPane($dimension, ImageDrawingPane::GIF));

        file_put_contents($filename = 'tree.gd.gif', $export->save());

        file_put_contents($filename.'.html', '<html><body style="margin:0px;"><img src='.$filename.'></body></html>');
    }

    if($doExport['JPG'])
    {
        $export = $plotter->plot($style, new GdLibDrawingPane($dimension, ImageDrawingPane::JPG));

        file_put_contents($filename = 'tree.gd.jpg', $export->save());

        file_put_contents($filename.'.html', '<html><body style="margin:0px;"><img src='.$filename.'></body></html>');
    }

    if($doExport['PNG'])
    {
        $export = $plotter->plot($style, new GdLibDrawingPane($dimension, ImageDrawingPane::PNG));

        file_put_contents($filename = 'tree.gd.png', $export->save());

        file_put_contents($filename.'.html', '<html><body style="margin:0px;"><img src='.$filename.'></body></html>');
    }
}

if($doExport['IMAGICK'])
{
    $export = $plotter->plot($style, new ImagickDrawingPane($dimension));

    file_put_contents($filename = 'tree.png', $export->save());

    file_put_contents($filename.'.html', '<html><body style="margin:0px;"><img src='.$filename.'></body></html>');
}

if($doExport['SVG'])
{
    $export = $plotter->plot($style, new SvgDrawingPane($dimension));

    file_put_contents('tree.svg', $export->save());
}

if($doExport['MAP'])
{
    $export = $plotter->plot($style, new HtmlMapDrawingPane($dimension, $mapName='rtTreeMap'));

    file_put_contents('tree.map.html', '<html><body style="margin:0px;">'.'<img src="tree.gd.gif" style="border:none;" usemap="#'.$mapName.'">'.$export->save());
}

if($doExport['HTML'])
{
    $export = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
       "http://www.w3.org/TR/html4/loose.dtd"><style>'.PHP_EOL.
                    '.rt_node'.PHP_EOL.
                    '{'.PHP_EOL.
                        //'z-index:         10;'.
                        'position:          absolute;'.PHP_EOL.
                        'outline:           none;'.PHP_EOL.
                        'white-space:       nowrap;'.PHP_EOL.
                        'overflow:          hidden;'.PHP_EOL.
                        'font-family:       Monospace;'.PHP_EOL.
                        'font-size:         11px;'.PHP_EOL.
                    '}'.PHP_EOL.

                    /*'div.rt_node:hover'.PHP_EOL.
                    '{'.
                        'color:             white;'.PHP_EOL.
                        'display:           table;'.PHP_EOL.
                        'z-index:           1;'.PHP_EOL.
                        '//margin-left:     -10%;'.PHP_EOL.
                        '//margin-top:      5px;'.PHP_EOL.
                        'padding:           5px;'.PHP_EOL.
                        'border:            solid 5px black;'.PHP_EOL.
                        'opacity:           0.9;'.PHP_EOL.
                    '}'.PHP_EOL.*/

                    '.nodeData'.
                    '{'.
                        'width:             100%;'.PHP_EOL.
                        'height:            100%;'.PHP_EOL.
                        'overflow:          hidden'.PHP_EOL.
                        ':          hidden'.PHP_EOL.
                    '}'.PHP_EOL.

                    '.rt_edge'.PHP_EOL.
                    '{'.
                        'position:          absolute;'.PHP_EOL.
                    '}'.PHP_EOL.

                '</style>';

    $plotter->plot($style, $doc = new HtmlDrawingPane($dimension));

    if(!isset($_POST))
	$_POST = array();

    $points = array();

    foreach($_POST as $edge)
        $points = array($layout->getPosition($edge[0], $style)->moveBy(new Point(0, 4)),
                    $layout->getPosition($edge[0], $style)->moveBy(new Point(-15, 4)),
                    $layout->getPosition($edge[1], $style)->moveBy(new Point(-15, 4)),
                    $layout->getPosition($edge[1], $style)->moveBy(new Point(0, 4)));

    if(count($points) > 0)
      $doc->drawPolyLine(new \ArrayObject($points));

    $export .= $doc->save();

    if(PHP_SAPI !== 'cli')
        echo $export;

    file_put_contents('tree.html', $export);


}

var_dump('export done in :'.(microtime(TRUE) - $start));
var_dump(memory_get_usage() / (1024 * 1024));
var_dump(memory_get_peak_usage() / (1024 * 1024));
// test on export type compatibility
/*
$d = new SvgDrawingPane(200, 100);
$d->setStrokeWidth(3)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(5, 10), new \ws\loewe\Utils\Geom\Point(55, 10))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(57, 12), new \ws\loewe\Utils\Geom\Point(57, 32))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(55, 34), new \ws\loewe\Utils\Geom\Point(5, 34))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(3, 32), new \ws\loewe\Utils\Geom\Point(3, 12));
file_put_contents('test.svg', $d->save());

$d = new GdLibDrawingPane(200, 100, ImageDrawingPane::GIF);
$d->setStrokeWidth(3)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(5, 10), new \ws\loewe\Utils\Geom\Point(55, 10))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(57, 12), new \ws\loewe\Utils\Geom\Point(57, 32))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(55, 34), new \ws\loewe\Utils\Geom\Point(5, 34))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(3, 32), new \ws\loewe\Utils\Geom\Point(3, 12));
file_put_contents('test.gif', $d->save());

$d = new SvgDrawingPane(300, 300);
$d->setStrokeWidth(1)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 10), new \ws\loewe\Utils\Geom\Point(100, 50))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 290), new \ws\loewe\Utils\Geom\Point(200, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 290), new \ws\loewe\Utils\Geom\Point(100, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 10), new \ws\loewe\Utils\Geom\Point(200, 50));
file_put_contents('test2.svg', $d->save());

$d = new GdLibDrawingPane(300, 300, ImageDrawingPane::GIF);
$d->setStrokeWidth(1)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 10), new \ws\loewe\Utils\Geom\Point(100, 50))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 290), new \ws\loewe\Utils\Geom\Point(200, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 290), new \ws\loewe\Utils\Geom\Point(100, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 10), new \ws\loewe\Utils\Geom\Point(200, 50));
file_put_contents('test2.gif', $d->save());
file_put_contents('test2.html', '<style>*{margin:0px;}</style><img src="test2.gif">');

$d = new SvgDrawingPane(300, 300);
$d->setStrokeWidth(3)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 10), new \ws\loewe\Utils\Geom\Point(100, 50))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 290), new \ws\loewe\Utils\Geom\Point(200, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 290), new \ws\loewe\Utils\Geom\Point(100, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 10), new \ws\loewe\Utils\Geom\Point(200, 50));
file_put_contents('test3.svg', $d->save());

$d = new GdLibDrawingPane(300, 300, ImageDrawingPane::GIF);
$d->setStrokeWidth(3)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 10), new \ws\loewe\Utils\Geom\Point(100, 50))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 290), new \ws\loewe\Utils\Geom\Point(200, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 290), new \ws\loewe\Utils\Geom\Point(100, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 10), new \ws\loewe\Utils\Geom\Point(200, 50));
file_put_contents('test3.gif', $d->save());
file_put_contents('test3.html', '<style>*{margin:0px;}</style><img src="test3.gif">');
*/

/*
$d = new SvgDrawingPane(200, 100);
$d->setStrokeWidth(1)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(5, 10), new \ws\loewe\Utils\Geom\Point(55, 10))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(57, 12), new \ws\loewe\Utils\Geom\Point(57, 32))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(55, 34), new \ws\loewe\Utils\Geom\Point(5, 34))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(3, 32), new \ws\loewe\Utils\Geom\Point(3, 12));
file_put_contents('test0.svg', $d->save());

$d = new ImagickDrawingPane(200, 100);
$d->setStrokeWidth(1)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(5, 10), new \ws\loewe\Utils\Geom\Point(55, 10))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(57, 12), new \ws\loewe\Utils\Geom\Point(57, 32))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(55, 34), new \ws\loewe\Utils\Geom\Point(5, 34))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(3, 32), new \ws\loewe\Utils\Geom\Point(3, 12));
file_put_contents('test0.png', $d->save());
file_put_contents('test0.html', '<style>*{margin:0px;}</style><img src="test0.png">');


$d = new SvgDrawingPane(300, 300);
$d->setStrokeWidth(2)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 10), new \ws\loewe\Utils\Geom\Point(100, 50))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 290), new \ws\loewe\Utils\Geom\Point(200, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 290), new \ws\loewe\Utils\Geom\Point(100, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 10), new \ws\loewe\Utils\Geom\Point(200, 50));
file_put_contents('test2.svg', $d->save());

$d = new ImagickDrawingPane(300, 300);
$d->setStrokeWidth(1)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 10), new \ws\loewe\Utils\Geom\Point(100, 50))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 290), new \ws\loewe\Utils\Geom\Point(200, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 290), new \ws\loewe\Utils\Geom\Point(100, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 10), new \ws\loewe\Utils\Geom\Point(200, 50));
file_put_contents('test2.png', $d->save());
file_put_contents('test2.html', '<style>*{margin:0px;}</style><img src="test2.png">');

$d = new SvgDrawingPane(300, 300);
$d->setStrokeWidth(4)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 10), new \ws\loewe\Utils\Geom\Point(100, 50))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 290), new \ws\loewe\Utils\Geom\Point(200, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 290), new \ws\loewe\Utils\Geom\Point(100, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 10), new \ws\loewe\Utils\Geom\Point(200, 50));
file_put_contents('test3.svg', $d->save());

$d = new ImagickDrawingPane(300, 300);
$d->setStrokeWidth(3)
        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 10), new \ws\loewe\Utils\Geom\Point(100, 50))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 255, 0))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 290), new \ws\loewe\Utils\Geom\Point(200, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(0, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(10, 290), new \ws\loewe\Utils\Geom\Point(100, 250))

        ->setStrokeColor(new \ws\loewe\Utils\Color\RgbColor(255, 0, 255))
        ->drawLine(new \ws\loewe\Utils\Geom\Point(290, 10), new \ws\loewe\Utils\Geom\Point(200, 50));
file_put_contents('test3.png', $d->save());
file_put_contents('test3.html', '<style>*{margin:0px;}</style><img src="test3.png">');
*/


function getRoot($type = null)
{
    if($type === 'FileSystem')
    {
        $root = new Directory(new SplFileInfo(INSTALLATION_FOLDER));

        $root->expand(  function($current, $key, $innerIterator)
                {
                    return true;
                });
    }

    else if($type === 'DOT')
    {
        $transformer = new DotTransformer('ART.dot');

        $root = $transformer->transform();
    }

    else if($type === 'JSON')
    {
        $ast = json_decode(file_get_contents("dump.json"), TRUE);

        $root = new TreeNode($ast['type']);

        buildAST($root, $ast['sig']);
    }

    else
    {
        $root = new TreeNode('ROOT');

        if(!true)
        {
            $tree = unserialize(file_get_contents('tree.ser'));
            $root = $tree[0];
        }
        else
        {
            //$root->populateRandomly(1000, 20);
            echo "\n creating tree ...";
            file_put_contents('tree.ser', serialize($root->populateRandomly(16000, 2)));
            echo "\ndone!";
        }
    }

    return $root;
}

function buildAST(TreeNode $node, $ast)
{
    foreach($ast as $child)
    {
        $data = $child['type'];
        if(!is_array($child['sig']))
            $data = $data."\n".$child['sig'];

        $childNode = new TreeNode($data);

        $node->appendChild($childNode);

        if(is_array($child['sig']))
            buildAST($childNode, $child['sig']);
    }
}

function getFontMetric(TextStyle $textStyle)
{
    if(PHP_SAPI == 'cli')
        $url = new Url('http', 'localhost', '', 9999);
    else
        $url = new Url('http', 'localhost', '/ReingoldTilford/FontManagerServiceEndpoint.php', null);

    // always delete the file in test-mode
    //if(file_exists('D:\\workspace//programming//PHP//ReingoldTilford//Utils//Font//fontMetrics.ser'))
    //    unlink('D:\\workspace//programming//PHP//ReingoldTilford//Utils//Font//fontMetrics.ser');

    $fontManager = new FontManager(FontMetricServiceFactory::createFontMetricService($url));

    return $fontManager->getFontMetric($textStyle);
}

function getStrings($root, $type)
{
    $strings = new ArrayObject();

    if($type === 'FileSystem')
    {
        $todo = array($root);
        while(count($todo) > 0)
        {
            $current = array_pop($todo);

            $strings[] = $current->getBasename();

            if($current instanceof Directory)
            {
                $i = 0;
                while(($child = $current->getChild($i)) != null)
                {
                    $todo[] = $child;
                    $i++;
                }
            }
        }
    }

    else if($type === 'DOT' || $type === 'JSON')
    {
        foreach(new \RecursiveIteratorIterator($root->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $node)
            $strings[] = $node->getData();
    }
    else
    {
        foreach(new \RecursiveIteratorIterator($root->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $node)
        {
            $data = null;

            for($i = 0, $limit = rand(5, 20); $i < $limit; $i++)
            {
                $r1 = abs(rand());
                $r2 = abs(rand());
                $r = rand(min($r1, $r2), 7 * max($r1, $r2));
                $data = $data.chr(rand(65, 125));
                if(rand() % 4 == 1)
                    $data = $data.\PHP_EOL;
            }

            $node->setData($data);

            $strings[] = $data;
        }
    }

    return $strings;
}
