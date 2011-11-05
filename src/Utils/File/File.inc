<?php

namespace Utils\File;

/**
 * This class represents a file in the file system.
 */
class File extends FileSystemObject
{
    /**
     * This method acts as the constructor of the class.
     *
     * @param \SplFileInfo $fileInfo the file information object associated with this file
     */
    public function __construct(\SplFileInfo $fileInfo)
    {
        parent::__construct($fileInfo);
    }
}