<?php

namespace Utils\Http;

/**
 * This class encapsulates a HTTP GET request.
 */
class HttpGetRequest extends HttpRequest
{
    /**
     * delimter that identifies, where the parameter string of a HTTP GET request starts
     *
     * @var string
     */
    private static $URL_PARAMETER_DELIMITER = "?";

    /**
     * This method acts as the constructor of the class
     *
     * @param string $rawRequest the raw HTTP GET request
     */
    public function __construct($rawRequest)
    {
        parent::__construct($rawRequest);
    }

    /**
     * This method extracts and returns the parameter string from the raw HTTP GET request.
     *
     * @return string the parameter string from the raw HTTP GET request or null if it was nout found
     */
    protected function getParamterString()
    {
        // get the position of the end of the HTTP header
        if(($urlParameterStart = strpos($this->rawRequest, self::$URL_PARAMETER_DELIMITER)) !== FALSE)
        {
            $start = $urlParameterStart + strlen(self::$URL_PARAMETER_DELIMITER);

            $end = strpos($this->rawRequest, ' ', $start);

            return substr($this->rawRequest, $start, $end - $start);
        }

        else
            return null;
    }
}