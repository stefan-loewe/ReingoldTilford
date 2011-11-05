<?php

namespace Utils\Autoload;

/**
 * This class represents an exception, which is thrown when the autoloader is unable to locate a source file that holds the definition of a class or interface.
 */
class SourceFileNotFoundException extends \RuntimeException
{
    /**
     * This method acts as the constructor for the class.
     *
     * @param string $name the name of the class/interface which could not be found.
     */
    public function __construct($name)
    {
        $this->message = 'No source file holding the class/interface definition of the class/interface "'.$name.'" were found!';
    }
}