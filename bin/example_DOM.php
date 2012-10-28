<?php

/**
 * This example exports the DOM tree of any given web site's HTML code to an SVG image.
 *
 * Just direct your browser to this file.
 */

use \ws\loewe\ReingoldTilford\Algorithms\ReingoldTilfordAlgorithm;
use \ws\loewe\ReingoldTilford\Models\DomTreeModel;
use \ws\loewe\ReingoldTilford\Plotters\Plotter;
use \ws\loewe\ReingoldTilford\Styles\TreeStyle;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\SvgDrawingPane;
use \ws\loewe\Utils\Color\RgbColor;

require __DIR__.'/../vendor/autoload.php';

header("Content-type: image/svg+xml");

$htmlBody = getHtmlBodyElement('http://www.phpclasses.org/');

// get the default style for a tree ...
$style = new TreeStyle();
// ... but do a bit of customisation
$style->shapeStyle->setWidth(48);
$style->shapeStyle->setHeight(30);
$style->shapeStyle->setColor(new RgbColor(232, 232, 232));
$style->shapeStyle->border->setWidth(3);
$style->shapeStyle->border->setColor(new RgbColor(50, 140, 198));
$style->edgeStyle->setWidth(3);

// get a new instance of the algorithm
$algorithm  = new ReingoldTilfordAlgorithm();

// get the model for the DOM tree, so that the algorithm can traverse it
$model      = new DomTreeModel($htmlBody);

// calculate the layout of the tree
$layout     = $algorithm->getLayout($model, $style->orientation)->normalize();

// pass it to the plotter
$plotter    = new Plotter($model, $layout, null, null, null);

// create a new SVG document
$document   = new SvgDrawingPane($layout->getDimension($style));

// plot the tree onto the document
$plotter->plot($style, $document);

// print the document
echo $document->save();

/**
 * This helper function gets the body element from a given website
 *
 * Obviously, this call can fail, but for mere demonstration purposes, any error handling has been left out for brevity.
 *
 * @param type $url
 * @return type
 */
function getHtmlBodyElement($url)
{
    error_reporting(0);
    $domDocument = new DOMDocument();

    $domDocument->loadHTML(file_get_contents($url));
    error_reporting(E_ALL | E_STRICT);

    return $domDocument->getElementsByTagName('body')->item(0);
}