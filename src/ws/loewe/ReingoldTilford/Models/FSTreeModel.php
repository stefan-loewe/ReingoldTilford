<?php

namespace ws\loewe\ReingoldTilford\Models;

use \ws\loewe\Utils\File\Directory;
use \ws\loewe\Utils\File\File;

/**
 * This class acts as ITreeModel implementation for the FileSystemObject classes (@see Utils\File\), so they can be used with ReingoldTilford algorithm.
 */
class FSTreeModel extends TreeModel
{
    /**
     * This method acts as the constructor of the TreeNodeTreeModel.
     *
     * @param object $root the root of the tree
     */
    public function __construct($root)
    {
        parent::__construct($root);
    }

    /**
     * This method returns the child of the given parent at the given index.
     *
     * @param object $parent the parent node
     * @param int $index the index
     */
    public function getChild($parent, $index)
    {
        return $parent->getChild($index);
    }

    /**
     * This method returns the number of children of the given parent.
     *
     * @param object $parent the parent node
     * @return int the number of children of the given parent
     */
    public function getChildCount($parent)
    {
        $count = 0;

        if(!$this->isLeaf($parent))
        {
            while($parent->getChild($count) != null)
                $count++;
        }

        return $count;
    }

    /**
     * This method returns the index of the given child in the given parent.
     *
     * @param object $parent the parent node
     * @param object $child the cild node
     * @return int the index of the given child in the given parent
     */
    public function getIndexOfChild($parent, $child)
    {
        if(!$this->isLeaf($parent))
        {
            for($i = 0, $count < $this->getChildCount($parent); $i < $count; $i++)
            {
                if($this->getChild($parent, $i) == $child)
                    return $i;
            }
        }

        return -1;
    }

    /**
     * This method returns true if the given node is a leaf.
     *
     * @param object $node the node
     * @return boolean true, if the the node is a leaf, else false
     */
    public function isLeaf($node)
    {
        return $node instanceof File ||
            ($node instanceof Directory && $node->getChild(0) == null);
    }

    /**
     *
     */
    public function getLabel($node)
    {
        return $node->__toString();
    }
}