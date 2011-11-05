<?php

namespace Utils\Logging;

/**
 * This class acts as a logger which prints to the standard output.
 */
class Logger
{
    /**
     * the identifier for the log level ALL
     */
    const ALL                   = -1;

    /**
     * the identifier for the log level OFF
     */
    const OFF                   = 0;

    /**
     * the identifier for the log level INFO
     */
    const INFO                  = 1;

    /**
     * the identifier for the log level WARNING
     */
    const WARNING               = 2;

    /**
     * the identifier for the log level ERROR
     */
    const ERROR                 = 4;

    /**
     * the identifier for the log level DEBUG
     */
    const DEBUG                 = 8;

    /**
     * the current log level of the logger
     *
     * @var int
     */
    private static $CURRENT_SEVERITY    = 15;

    /**
     * the target of the logging output
     *
     * @var string
     */
    protected static $target            = 'php://stdout';

    /**
     * This method logs the given message to the respective target, if the currrent severity includes the passed severity.
     *
     * @param int $severity the log level of the log message
     * @param string $message the log message
     */
    public static function log($severity, $message)
    {
        if($severity === self::ALL || self::isSevere($severity))
            file_put_contents(static::$target, PHP_EOL.date('YmdHis').': Logger: '.$message, FILE_APPEND);
    }

    /**
     * This method sets the log level of the logger.
     *
     * @param int $severity the new severity of the logger
     */
    public static function setLogLevel($severity)
    {
        self::$CURRENT_SEVERITY = $severity;
    }

    /**
     * This method decides whether or not the passed severity is included in the current severity of the logger.
     *
     * @param int $severity the severity to decide upon
     * @return boolean true, if the passed log level is included in the loggers severity, false if not
     */
    protected static function isSevere($severity)
    {
        return ($severity & static::$CURRENT_SEVERITY) == $severity;
    }
}