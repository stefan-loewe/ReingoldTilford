<?php

/**
 * This example exports the directory structure of the installation folder of this package to HTML.
 *
 * Just direct your browser to this file.
 */

use \ws\loewe\ReingoldTilford\Algorithms\ReingoldTilfordAlgorithm;
use \ws\loewe\ReingoldTilford\Models\FSTreeModel;
use \ws\loewe\ReingoldTilford\Plotters\Plotter;
use \ws\loewe\ReingoldTilford\Styles\TreeStyle;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\HtmlDrawingPane;
use \ws\loewe\Utils\Graphics2D\Shapes\Styles\TextStyle;
use \ws\loewe\Utils\File\Directory;
use \ws\loewe\Utils\Font\FontManager;
use \ws\loewe\Utils\Font\FontMetricServices\FontMetricServiceFactory;
use \ws\loewe\Utils\URL\Url;

require __DIR__.'/../vendor/autoload.php';

// this example only runs through a web server
if(PHP_SAPI === 'cli')
{
    echo 'This eample can only be run through a web server.'."\n".'This script will exit now.';
    exit(-1);
}

// get the directory information
$rootDirectory = new Directory(new \SplFileInfo(__DIR__.'/../'));

// but filter out all hidden files, i.e. those starting with a dot
$filter = function($current) {
                    return strpos($current->getFilename(), '.') !== 0;
                };
$rootDirectory->expand($filter);

$basenames  = getFilenames($rootDirectory);

// get the default style for a tree
$style      = new TreeStyle();

// get the respective font metric, to be able to determine the maximal string length
// this uses the Browser's rendering engine plus an AJAX call - which might fail
// see FontManager::createFontMetric and subsequent calls for more information
$metric     = getFontMetric($style->textStyle);

// set the width and height of the nodes of the tree
$style->shapeStyle->setWidth($metric->getMaxWidth($basenames));
$style->shapeStyle->setHeight($metric->getMaxHeight($basenames));

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
 * This helper method returns the font metric for a given text style.
 *
 * In order to get the metrics of the font, back into the calling script, an AJAX request is pointed to to backend to initialise the font metric.
 *
 * @param TextStyle $textStyle the text style for which to font metric is needed
 * @return FontMetric
 */
function getFontMetric(TextStyle $textStyle)
{
    $currentUrl = Url::createCurrentUrlFromServerInfo();

    $endpointUrl = $currentUrl->setPath(str_replace('example_directory_tree.php', 'FontManagerServiceEndpoint.php', $currentUrl->getPath()));

    $fontManager = new FontManager(FontMetricServiceFactory::createFontMetricService($endpointUrl));

    return $fontManager->getFontMetric($textStyle);
}

/**
 * This helper method gets the names of all folders and files of the given directory.
 *
 * @param Directory $root
 * @return ArrayObject the names of all folders and files of the given directory
 */
function getFilenames(Directory $root)
{
    $basenames = new \ArrayObject();

    foreach($root->getDecendants() as $node)
        $basenames[] = $node->getBasename();

    return $basenames;
}

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

            '.rt_edge'.PHP_EOL.
            '{'.
                'position:          absolute;'.PHP_EOL.
            '}'.PHP_EOL.

            '.text'.PHP_EOL.
            '{'.
                'width:             '.$treeStyle->shapeStyle->width.'px;'.PHP_EOL.
                'text-align:        center;'.PHP_EOL.
            '}'.PHP_EOL.
        '</style>';
}
