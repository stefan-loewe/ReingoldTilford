<?php

namespace Utils\Font\FontMetricServices;

use Utils\Font\FontManager;
use Utils\Graphics2D\Shapes\Styles\TextStyle;
use Utils\Sockets\CallbackServer;
use Utils\URL\Url;
use Utils\Http\HttpRequest;
use Utils\Font\FontMetric;

/**
 * This socket-based font metric service operates upon a callback server (@see \Utils\Sockets\CallbackServer).
 */
class AdvancedFontMetricSocketService extends FontMetricSocketService
{
    /**
     * This method acts as the constructor of the class
     *
     * @param Url $url the URL of the socket to communicate with
     */
    public function __construct(Url $url)
    {
        parent::__construct($url);
    }

    /**
     * This method start a call back HTTP server, waits for and consumes exactly one request to extract and add a font definition, and stops therafter.
     *
     * This method expects the request to be a valid \Utils\Http\HttpRequest which contains a valid font definition.
     *
     * @param TextStyle $textStyle the text style for which the font definition is needed
     * @param FontManager $fontManager the font manager to which the font definition is added to
     */
    public function startSocketServer(TextStyle $textStyle, FontManager $fontManager)
    {
        // the call back receives a HTTP request, and adds the extracted font definition to the font manager for the respective text style
        $callback = function(HttpRequest $request) use ($fontManager, $textStyle)
                    {
                        $keyValuePairs = $request->getKeyValuePairs();

                        $fontManager->addFontMetric(new FontMetric($textStyle, explode(',', $keyValuePairs['fontDefinitions'])));

                        // stop the callback server
                        return false;
                    };

        $server = new CallbackServer($this->url->getHost(), $this->url->getPort(), null);

        $server->register($callback)->run();
    }
}