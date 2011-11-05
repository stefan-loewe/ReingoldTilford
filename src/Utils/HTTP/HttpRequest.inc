<?php

namespace Utils\Http;

/**
 * This class is an abstract base class for HTTP requests.
 */
abstract class HttpRequest
{
    /**
     * the raw HTTP request
     *
     * @var string
     */
    protected $rawRequest = null;

    /**
     * This method acts as the constructor of the class.
     *
     * @param string $rawRequest the raw HTTP request
     */
    public function __construct($rawRequest)
    {
        $this->rawRequest = $rawRequest;
    }

    /**
     * This mehtod extracts the key-value pairs of a HTTP request.
     *
     * The name of a parameter is a key of the ArrayObject and points to its according value.
     *
     * @param string $paramterString the parameter string from the HTTP request
     * @return ArrayObject the parameters of the HTTP request as key-value pairs
     */
    private function extractKeyValuePairs($paramterString)
    {
        $keyValuePairs = new \ArrayObject();

        if($paramterString != null)
        {
            foreach(($parameters = explode('&', $paramterString)) as $parameter)
            {
                if(strlen($parameter) > 0)
                {
                    $keyValue = explode('=', $parameter);

                    $keyValuePairs[$keyValue[0]] = isset($keyValue[1]) ? $keyValue[1] : null;
                }
            }
        }

        return $keyValuePairs;
    }

    /**
     * This method returns the parameters of the request as key-value pairs.
     *
     * The name of a parameter is a key of the ArrayObject and points to its according value.
     *
     * @return ArrayObject the parameters of the request as key-value pairs
     */
    public function getKeyValuePairs()
    {
        return $this->extractKeyValuePairs($this->getParamterString());
    }

    /**
     * This method returns the raw request of the HTTP request.
     *
     * @return string the raw request of the HTTP request
     */
    public function getRawRequest()
    {
        return $this->rawRequest;
    }
}