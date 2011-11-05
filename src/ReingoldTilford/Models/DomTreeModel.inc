<?php

namespace ReingoldTilford\Models;

/**
 * This class acts as ITreeModel implementation for the TreeNode class (@see Lib\TreeNode), so this can be used with ReingoldTilford algorithm.
 */
class DomTreeModel extends TreeModel
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
        return $parent->childNodes->item($index);
    }

    /**
     * This method returns the number of children of the given parent.
     *
     * @param object $parent the parent node
     * @return int the number of children of the given parent
     */
    public function getChildCount($parent)
    {
        if($parent->hasChildNodes())
            return $parent->childNodes->length;

        else
            return 0;
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
        foreach($parent->childNodes as $index => $currentChild)
        {
            if($currentChild == $child)
                return $index;
        }

        return -1;
    }

    /**
     * This method returns the root of the tree.
     *
     * @return object the root of the tree
     */
    public function getRoot()
    {
        return $this->root;
    }

    /**
     * This method returns true if the given node is a leaf.
     *
     * @param object $node the node
     * @return boolean true, if the the node is a leaf, else false
     */
    public function isLeaf($node)
    {
        return $this->getChildCount($node) === 0;
    }

    /**
     * This method returns the label associated with the DOM node, i.e. the node name.
     *
     * As the PHP DOMNode class does not provide a __toString method, a custom implementation is needed.
     *
     */
    public function getLabel($node)
    {
        return substr($node->nodeName, 0, 6);
    }
}