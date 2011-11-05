<?php

namespace Utils\Http;

/**
 * This class encapsulates a HTTP POST request.
 */
class HttpPostRequest extends HttpRequest
{
    /**
     * delimter that identifies, where the parameter string of a HTTP POST request starts
     *
     * @var string
     */
    private static $HEADER_DELIMITER = "\r\n\r\n";

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
     * This method extracts and returns the parameter string from the raw HTTP POST request.
     *
     * @return string the parameter string from the raw HTTP POST request or null if it was nout found
     */
    protected function getParamterString()
    {
        // get the position of the end of the HTTP header
        if(($headerEndPos = strpos($this->rawRequest, self::$HEADER_DELIMITER)) !== FALSE)
            return substr($this->rawRequest, $headerEndPos + strlen(self::$HEADER_DELIMITER));

        else
            return null;
    }
}