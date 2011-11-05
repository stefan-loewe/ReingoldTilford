<?php

namespace Utils\Logging;

/**
 * This class acts as a logger which prints to a file.
 */
class FileLogger extends Logger
{
    /**
     * the target of the file logger
     *
     * @var string
     */
    protected static $target = 'log.txt';

    /**
     * This method sets the file name of the log file to use.S
     *
     * @param string $filename the file name of the file where to log to
     */
    public static function setLogFilename($filename)
    {
        self::$target = $filename;
    }
}