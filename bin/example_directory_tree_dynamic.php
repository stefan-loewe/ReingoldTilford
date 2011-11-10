<?php

/**
 * This example exports the directory structure of the installation folder of this package to HTML, with some interactive components.
 *
 * Just direct your browser to this file.
 */

use Utils\Autoload\Autoloader;
use ReingoldTilford\Algorithms\ReingoldTilfordAlgorithm;
use ReingoldTilford\Models\FSTreeModel;
use ReingoldTilford\Plotters\Plotter;
use ReingoldTilford\Styles\TreeStyle;
use Utils\Graphics2D\DrawingPanes\HtmlDrawingPane;
use Utils\Graphics2D\Shapes\Styles\TextStyle;
use Utils\Geom\Dimension;
use Utils\File\Directory;
use Utils\Font\FontManager;
use Utils\Font\FontMetricServices\FontMetricServiceFactory;
use Utils\URL\Url;

require_once __DIR__.'/../app/bootstrap.inc';

// get the directory information
$rootDirectory = new Directory(new SplFileInfo(INSTALLATION_FOLDER));

$rootDirectory->expand(  function($current, $key, $innerIterator)
                {
                    return true;
                });

// get the default style for a tree
$style = new TreeStyle();
$style->shapeStyle->setWidth(8);
$style->shapeStyle->setHeight(5);
$style->shapeStyle->setColor(new \Utils\Color\RgbColor(251, 247, 180));
$style->shapeStyle->border->setColor(new \Utils\Color\RgbColor(228, 212, 128));
$style->edgeStyle->setColor(new \Utils\Color\RgbColor(200, 200, 200));

// get a new instance of the algorithm
$algorithm  = new ReingoldTilfordAlgorithm();

// get the model for the root directory, so that the algorithm can traverse it
$model      = new FSTreeModel($rootDirectory);

// get the layout of the tree
$layout     = $algorithm->getLayout($model, $style->orientation)->normalize();

// pass it to the plotter
$plotter    = new Plotter($model, $layout, null, null, null);

// create a new document
$document = new HtmlDrawingPane($layout->getDimension($style));

// plot it onto the document
$plotter->plot($style, $document);

// print the html head
printHtmlHead($style);

// print the document
echo $document->save();

/**
 * This helper method prints the html head, e.g. the CSS styles in use.
 */
function printHtmlHead(TreeStyle $treeStyle)
{
    echo '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"'.PHP_EOL.
       '"http://www.w3.org/TR/html4/loose.dtd">'.PHP_EOL.
        '<style>'.PHP_EOL.
            '.rt_node'.PHP_EOL.
            '{'.PHP_EOL.
                'position:          absolute;'.PHP_EOL.
            '}'.PHP_EOL.

            '.sty0:hover'.PHP_EOL.
            '{'.PHP_EOL.
                'width:             240px;'.PHP_EOL.
                'height:            100px;'.PHP_EOL.
                'opacity:           0.5;'.PHP_EOL.
                'z-index:           10;'.PHP_EOL.
                'border:            solid 2px black;'.PHP_EOL.
            '}'.PHP_EOL.

            '.rt_edge'.PHP_EOL.
            '{'.
                'position:          absolute;'.PHP_EOL.
            '}'.PHP_EOL.

            '.text'.PHP_EOL.
            '{'.
                'width:             0px;'.PHP_EOL.
                'overflow:          hidden;'.PHP_EOL.
            '}'.PHP_EOL.

            '.sty0:hover + .text'.PHP_EOL.
            '{'.
                'width:             240px;'.PHP_EOL.
                'overflow:          visible;'.PHP_EOL.
                'z-index:           1;'.PHP_EOL.
                'text-align:        center;'.PHP_EOL.
            '}'.PHP_EOL.

        '</style>';
}
