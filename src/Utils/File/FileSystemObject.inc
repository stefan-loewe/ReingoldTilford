<?php

namespace Utils\File;

/**
 * This class represents an abstract object in the file system.
 */
abstract class FileSystemObject implements IFileSystemObject
{
    /**
     * the file information object associated with this file system object
     *
     * @var \SplFileInfo
     */
    protected $fileInfo    = null;

    /**
     * the parent directory of the file system object
     *
     * @var Directory
     */
    protected $parent      = null;

    /**
     * This method acts as the constructor of the class.
     *
     * @param \SplFileInfo $fileInfo the file information object associated with this file system object
     */
    public function __construct(\SplFileInfo $fileInfo)
    {
        $this->fileInfo = $fileInfo;
    }

    /**
     * This method returns the base name of the file system object.
     *
     * @return string the base name of the file system object
     */
    public function getBasename()
    {
        return $this->fileInfo->getBasename(null);
    }

    /**
     * This method returns the path name of this file system object.
     *
     * @return string the path name of this file system object
     */
    public function getPathname()
    {
        return $this->fileInfo->getPathname();
    }

    /**
     * This method returns the parent directory of this file system object.
     *
     * @return Directory the parent directory of this file system object
     */
    public function getParent()
    {
        return $this->parent;
    }

    /**
     * This method returns the string representation of the file system object, i.e. its base name.
     *
     * @return string the base name of the file system object as string
     */
    public function __toString()
    {
        return $this->fileInfo->getBasename(null);
    }
}