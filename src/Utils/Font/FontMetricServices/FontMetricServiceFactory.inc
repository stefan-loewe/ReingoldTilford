<?php

namespace Utils\Font\FontMetricServices;

use Utils\URL\Url;

/**
 * This class acts as factory for creating font metric services.
 */
class FontMetricServiceFactory
{
    /**
     * This method creates a FontMetricService - whether or the script is running through the command line, a FontMetricSocketService or a FontMetricWebServerService is returned.
     *
     * @param Url $url the URL of the underlying service endpoint, e.g. localhost:9987 for a FontMetricSocketService, or http://localhost/endpoint.php for a FontMetricWebServerService.
     * @return FontMetricWebServerService the created FontMetricService
     */
    public static function createFontMetricService(Url $url)
    {
        if(self::isCommandLineInvocation())
            $service = (true) ? new SimpleFontMetricSocketService($url) : new AdvancedFontMetricSocketService($url);

        else
            $service = new FontMetricWebServerService($url);

        return $service;
    }

    /**
     * This method determines whether the script was called from the command line or not.
     *
     * @return boolean true, if the script was called from the command line, else false
     */
    private static function isCommandLineInvocation()
    {
        return PHP_SAPI === 'cli';
    }
}
