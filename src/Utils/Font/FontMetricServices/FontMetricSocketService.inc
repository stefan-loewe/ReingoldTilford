<?php

namespace Utils\Font\FontMetricServices;

use Utils\Font\FontManager;
use Utils\Graphics2D\Shapes\Styles\TextStyle;
use Utils\File\Directory;
use Utils\URL\Url;

/**
 * This is an abstract class for font metric services that operate via a socket connection.
 */
abstract class FontMetricSocketService extends FontMetricService
{
    /**
     * the file name of the font metric HTML file
     *
     * @var string
     */
    protected $fontMetricsHtml  = 'fontMetrics.html';

    /**
     * the URL of the socket
     *
     * @var Url
     */
    protected $url              = null;

    /**
     * This method acts as the constructor of the class
     *
     * @param Url $url the URL of the socket to communicate with
     */
    public function __construct(Url $url)
    {
        parent::__construct();

        $this->url  = $url;
    }

    /**
     * This method executes the respective service.
     *
     * It creates the respective HTML code by passing the service endpoint information, and starts a socket server, which handles the calls from the created HTML file once this is opened.
     *
     * @param TextStyle $textStyle the text style for which the font definition is needed
     * @param FontManager $fontManager the font manager to which the font definition is added to
     */
    public function execute(TextStyle $textStyle, FontManager $fontManager)
    {
        $html = $this->createHtml($textStyle, $this->url);

        file_put_contents(Directory::toRealPath(__DIR__.'/../').$this->fontMetricsHtml, $html);

        $this->startSocketServer($textStyle, $fontManager);
    }
}
