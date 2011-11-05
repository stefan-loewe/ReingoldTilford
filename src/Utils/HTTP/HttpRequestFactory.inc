<?php

namespace Utils\Http;

/**
 * This class acts as Factory for HTTP requests.
 *
 * This class acts as Factory for HTTP requests. When given a raw HTTP request, this class creates the appropriate subclass of HttpRequest (@see Utils\Http\HttpRequest).
 */
class HttpRequestFactory
{
    /**
     * keyword to identify a HTTP GET request
     *
     * @var string
     */
    private static $GET_TOKEN = 'GET';

    /**
     * keyword to identify a HTTP POST request
     *
     * @var string
     */
    private static $POST_TOKEN = 'POST';

    /**
     * This method determines, if the given raw HTTP request is a HTTP GET request.
     *
     * @param string $rawRequest the raw HTTP request
     * @return boolean true, if the given HTTP request is a HTTP GET request, else false
     */
    private static function isHttpGetRequest($rawRequest)
    {
        return strpos($rawRequest, self::$GET_TOKEN) === 0;
    }

    /**
     * This method determines, if the given raw HTTP request is a HTTP POST request.
     *
     * @param string $rawRequest the raw HTTP request
     * @return boolean true, if the given HTTP request is a HTTP POST request, else false
     */
    private static function isHttpPostRequest($rawRequest)
    {
        return strpos($rawRequest, self::$POST_TOKEN) === 0;
    }

    /**
     * This method creates the respective HTTP request object for the given raw request.
     *
     * @param string $rawRequest the raw HTTP request
     * @return HttpRequest an instance of HttpGetRequest or HttpPostRequest, based on the given raw request
     */
    public static function createRequest($rawRequest)
    {
        if(self::isHttpGetRequest($rawRequest))
            return new HttpGetRequest($rawRequest);

        else if(self::isHttpPostRequest($rawRequest))
            return new HttpPostRequest($rawRequest);

        else
            throw new \InvalidArgumentException('Could not determine type of (HTTP) request');

    }
}