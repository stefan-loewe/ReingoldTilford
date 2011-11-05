<?php

namespace Utils\Font\FontMetricServices;

use Utils\Font\FontManager;
use Utils\Graphics2D\Shapes\Styles\TextStyle;
use Utils\URL\Url;

/**
 * This font metric service can create font metrics when being invoked directly through the browser.
 */
class FontMetricWebServerService extends FontMetricService
{
    /**
     * the service endpoint, which is the target of the callback in the html code, that sends the text style information
     *
     * @var Url
     */
    private $serviceEndpoint = null;

    /**
     * This method acts as the constructor of the class.
     *
     * @param Url $url the service endpoint, which is the target of the callback in the html code, that sends the text style information
     */
    public function __construct(Url $url)
    {
        parent::__construct();

        $this->serviceEndpoint = $url;
    }

    /**
     * This method creates HTML and Javascript which is evaluated bz the browser, and then feeds back font metrics to the script.
     *
     * @param TextStyle $textStyle the text style for which the font definition is needed
     * @param FontManager $fontManager the font manager to which the font definition is added to
     */
    public function execute(TextStyle $textStyle, FontManager $fontManager)
    {
        echo $this->createHtml($textStyle, $this->serviceEndpoint);

        // be certain that the HTML snippet above is being printed immediatelly
        flush();
    }
}