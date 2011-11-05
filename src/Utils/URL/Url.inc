<?php

namespace Utils\URL;

/**
 * This class is a simple wrapper for an URL.
 */
class Url
{
    /**
     * the protocol of the URL
     *
     * @var string
     */
    private $protocol   = null;

    /**
     * the host address of the URL, e.g. a fully qualified name, or an IP address
     *
     * @var string
     */
    private $host       = null;

    /**
     * the path component of the URL
     *
     * @var string
     */
    private $path       = null;

    /**
     * the number port of the URL
     *
     * @var int
     */
    private $port       = null;

    /**
     * This method acts as the constructor of the class.
     *
     * @param string $protocol the protocol of the URL
     * @param string $host the host address of the URL, e.g. a fully qualified name, or an IP address
     * @param string $path the path component of the URL
     * @param int $port the number port of the URL, maybe null
     */
    function __construct($protocol, $host, $path, $port = null)
    {
        $this->protocol = $protocol;

        $this->host     = $host;

        $this->path     = $path;

        $this->port     = $port;
    }

    /**
     * This method return the protocol of the URL.
     *
     * @return string the protocol of the URL
     */
    public function getProtocol()
    {
        return $this->protocol;
    }

    /**
     * This method return the host address of the URL.
     *
     * @return string the host address of the URL
     */
    public function getHost()
    {
        return $this->host;
    }

    /**
     * This method return the path component of the URL.
     *
     * @return string the path component of the URL
     */
    public function getPath()
    {
        return $this->path;
    }

    /**
     * This method return the port number of the URL.
     *
     * @return int the port number of the URL, or null if it is not set
     */
    public function getPort()
    {
        return $this->port;
    }

    /**
     * This method sets the path of the URL.
     *
     * @param string $path the path to set
     * @return Url the URL with the given path set
     */
    public function setPath($path)
    {
        return new Url($this->protocol, $this->host, $path, $this->port);
    }

    /**
     * This method returns a string representation of the Url.
     *
     * @return string the string representation of the Url
     */
    public function __toString()
    {
        $result = $this->protocol.'://'.$this->host;

        if($this->port !== null)
            $result = $result.':'.$this->port;

        return $result.$this->path;
    }

    /**
     * This method creates and returns an URL that is represented in the PHP $_SERVER super global.
     *
     * @return Url the URL that is represented in the PHP $_SERVER super global
     */
    public static function createCurrentUrlFromServerInfo()
    {
        $protocol = strtolower($_SERVER['SERVER_PROTOCOL']);

        $protocol = substr($protocol, 0, strpos($protocol, '/'));

        // change from http to https
        if($protocol === 'http' && !empty($_SERVER['HTTPS']))
            $protocol = $protocol.'s';

        $host = $_SERVER['SERVER_NAME'];

        $path = $_SERVER['REQUEST_URI'];

        $port = $_SERVER['SERVER_PORT'];

        return new Url($protocol, $host, $path, $port);
    }
}