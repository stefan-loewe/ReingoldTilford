<?php

namespace ws\loewe\ReingoldTilford\Models;

/**
 * This class acts as a full TreeModel implementation for the TreeNode class (@see Lib\TreeNode), so this can be used with ReingoldTilford algorithm.
 */
class TreeNodeTreeModel extends TreeModel
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
        return $parent->getChildAtIndex($index);
    }

    /**
     * This method returns the number of children of the given parent.
     *
     * @param object $parent the parent node
     * @return int the number of children of the given parent
     */
    public function getChildCount($parent)
    {
        return $parent->getDegree();
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
        return $parent->getIndexOfChild($child);
    }

    /**
     * This method returns true if the given node is a leaf.
     *
     * @param object $node the node
     * @return boolean true, if the the node is a leaf, else false
     */
    public function isLeaf($node)
    {
        return $node->isLeaf();
    }
}