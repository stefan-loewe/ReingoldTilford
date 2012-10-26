<?php

namespace ws\loewe\ReingoldTilford\Models;

/**
 * This class acts as a full TreeModel implementation for the DefaultTreeNode class (@see DefaultTreeNode), so this can be used with ReingoldTilford algorithm.
 */
class DefaultTreeModel extends TreeModel
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
        $counter = 0;

        while($parent->getChild($counter) !== null)
            $counter++;

        return $counter;
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
        for($i = 0, $count = $this->getChildCount($parent); $i < $count; $i++)
        {
            if($child == $this->getChild($parent, $i))
                return $i;
        }

        return -1;
    }
}