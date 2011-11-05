<?php

namespace Utils\File;

/**
 * This class represents a directory in the file system. It contains a collection of children referencing files and other directories.
 */
class Directory extends FileSystemObject
{
    /**
     * the children of the directory
     *
     * @var \ArrayObject
     */
    protected $children = null;

    /**
     * This method acts as the constructor of the class.
     *
     * @param \SplFileInfo $fileInfo the file information object associated with this directory
     */
    public function __construct(\SplFileInfo $fileInfo)
    {
        parent::__construct($fileInfo);

        $this->children = new \ArrayObject();
    }

    /**
     * This method adds a child to this directory.
     *
     * @param FileSystemObject $child the child to add
     */
    public function addChild(FileSystemObject $child)
    {
        $this->children[]   = $child;

        $child->parent      = $this;
    }

    /**
     * This method gets the child from the given index.
     *
     * @param int $index the index from where to get the child
     * @return FileSystemObject the child at the given index or null if none exists at this index.
     */
    public function getChild($index)
    {
        return isset($this->children[$index]) ? $this->children[$index] : null;
    }

    /**
     * This method returns all the decendants of a Directory recursively.
     *
     * @return \ArrayObject of FileSystemObjects
     */
    public function getDecendants()
    {
        $children = new \ArrayObject();

        $todo = array($this);
        while(count($todo) > 0)
        {
            $current = array_pop($todo);

            $children[] = $current;

            if($current instanceof Directory)
            {
                $i = 0;
                while(($child = $current->getChild($i)) != null)
                {
                    $todo[] = $child;
                    $i++;
                }
            }
        }

        return $children;
    }

    /**
     * This method expands this directory to include all its subdirectories and files.
     * 
     * Currently, it does not include files and directories starting with a dot, e.g. .svn. This filtering cannot be customized before the arrival of PHP 5.4.
     *
     * @return Directory this directory, with all its subdirectories and files.
     */
    public function expand(\Closure $filter)
    {
        $folders = array();

        $folders[$this->getPathname()] = $this;

        $directoryIterator  = new \RecursiveDirectoryIterator($this->getPathname());

        // TODO; replace FileFilterIterator with CallbackFilterIterator in PHP 5.4
        $filterIterator     = new FileFilterIterator($directoryIterator, $filter);
        
        $fileSystemIterator = new \RecursiveIteratorIterator($filterIterator, \RecursiveIteratorIterator::SELF_FIRST);

        foreach($fileSystemIterator as $filePath => $fileInfo)
        {
            if($fileInfo->isDir())
            {
                $fso = new Directory($fileInfo);
                $folders[$fileInfo->getPathname()] = $fso;
            }
            else
                $fso = new File($fileInfo);

            // as some file system object are ignored (see above), check if parent of current element is available
            if(isset($folders[$fileInfo->getPath()]))
                $folders[$fileInfo->getPath()]->addChild($fso);
        }

        return $this;
    }

    /**
     * This method returns the absolute path to a given relative one. When calling this method for a directory, then the resulting absolute path will always have a trailing slash.
     *
     * Note, the referenced directory or file must exist physically prior to calling this method.
     *
     * @param string $relativePath the relative path to the file system object
     * @return string the absolute path for the given relative one
     */
    public static function toRealPath($relativePath)
    {
        $relativePath = str_replace('\\', DIRECTORY_SEPARATOR, $relativePath);
        $relativePath = str_replace('/', DIRECTORY_SEPARATOR, $relativePath);

        return is_dir($path = realpath($relativePath)) ? $path.DIRECTORY_SEPARATOR : $path;
    }
}

class FileFilterIterator extends \RecursiveFilterIterator
{
    private $filter = null ;

    public function __construct(\RecursiveDirectoryIterator $iterator, \Closure $filter = null)
    {
        parent::__construct($iterator);
        
        $this->filter = $filter;
    }

    public function accept()
    {
        return strpos($this->current()->getFilename(), '.') !== 0;
        /*
        if($this->filter === null)
            return true;
            
        else
            return $this->filter->__invoke($this->current(), $this->key(), $this->getInnerIterator());
         */
    }
}