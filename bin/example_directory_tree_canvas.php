<?php

/**
 * This example exports the directory structure of the installation folder of this package to HTML.
 *
 * Just direct your browser to this file.
 */

use Utils\Autoload\Autoloader;
use ReingoldTilford\Algorithms\ReingoldTilfordAlgorithm;
use ReingoldTilford\Models\FSTreeModel;
use ReingoldTilford\Plotters\Plotter;
use ReingoldTilford\Styles\TreeStyle;
use Utils\Graphics2D\DrawingPanes\HtmlCanvasDrawingPane;
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
                    return strpos($current->getFilename(), '.') !== 0;
                });
$basenames  = getFilenames($rootDirectory);

// get the default style for a tree
$style      = new TreeStyle();

// get a new instance of the algorithm
$algorithm  = new ReingoldTilfordAlgorithm();

// get the model for the root directory, so that the algorithm can traverse it
$model      = new FSTreeModel($rootDirectory);

// get the layout of the tree
$layout     = $algorithm->getLayout($model, $style->orientation)->normalize();

// pass it to the plotter
$plotter    = new Plotter($model, $layout, null, null, null);

// create a new document
$dim = $layout->getDimension($style);
$document = new HtmlCanvasDrawingPane($dim);

// plot it onto the document
$plotter->plot($style, $document);

echo '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"'.PHP_EOL.
       '"http://www.w3.org/TR/html4/loose.dtd">'.PHP_EOL.
        '<html><head><script>function draw(){';


// print the document
echo $document->save();
echo '}</script></head><body onload="draw();"><canvas id="rt_canvas" width="'.$dim->width.'" height="'.$dim->height.'">
</canvas></body></html>';

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

