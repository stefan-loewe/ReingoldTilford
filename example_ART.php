<?php

ini_set('memory_limit', '4294967296');

set_time_limit(0);

error_reporting(E_ALL | E_STRICT);

use Utils\Autoload\Autoloader;
use Utils\Tree\DotTransformer;
use ReingoldTilford\Models\DefaultTreeModel;
use ReingoldTilford\Algorithms\ReingoldTilfordAlgorithm;
use ReingoldTilford\Plotters\Plotter;
use ReingoldTilford\Styles\TreeStyle;
use Utils\Graphics2D\DrawingPanes\SvgDrawingPane;
use Utils\Geom\Dimension;

require_once 'bootstrap.inc';

$start = microtime(TRUE);

if(!isset($argv[1]))
    $argv[1] = 'ART_huge.dot';

$argv[1] = 'ART_small.dot';
$argv[1] = 'ART.dot';

$transformer = new DotTransformer($argv[1]);

$root = $transformer->transform();

$algorithm = new ReingoldTilfordAlgorithm();

$model = new \ReingoldTilford\Models\DefaultTreeModel($root);

echo "\n".date("Y.m.d H:i:s")." - creating layout ...";
$start = microtime(TRUE);
$layout = $algorithm->getLayout($model, TreeStyle::CENTERED)->normalize();
echo "\n".date("Y.m.d H:i:s")." - ... done";
var_dump('got layout in :'.(microtime(TRUE) - $start));

unset($algorithm);

// get the default style for a tree
$style = new TreeStyle();

$style->shapeStyle->setWidth(8);
$style->shapeStyle->setHeight(5);
$style->shapeStyle->setColor(new \Utils\Color\RgbColor(232, 232, 232));
$style->shapeStyle->border->setWidth(1);
$style->shapeStyle->border->setColor(new \Utils\Color\RgbColor(50, 140, 198));
$style->edgeStyle->setWidth(1);

// pass it to the plotter
$plotter    = new Plotter($model, $layout, null, null, null);

// create a new document
$document   = new SvgDrawingPane($layout->getDimension($style));

// plot it onto the document
$plotter->plot($style, $document);

$document->setStrokeColor(new Utils\Color\RgbColor(255, 0, 0));
foreach($_POST as $edges)
{
    $pos1 = $layout->getPosition($edges[0], $style);
    $pos2 = $layout->getPosition($edges[1], $style);
    $document->drawLine($pos1, $pos2);
    //echo "\ndarw line from $pos1 to $pos2, lenght = ".($pos2->y - $pos1->y);
}
// print the document
file_put_contents('tree.svg', $document->save());

var_dump('export done in :'.(microtime(TRUE) - $start));
var_dump(memory_get_usage() / (1024 * 1024));
var_dump(memory_get_peak_usage() / (1024 * 1024));
