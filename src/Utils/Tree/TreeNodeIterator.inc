<?php

namespace Utils\Tree;

/**
 * This class implements an iterator to visit the children of a tree node (@see \Lib\TreeNode)
 */
class TreeNodeIterator implements \Iterator
{
    /**
     * the children of the node to iterate over
     *
     * @var array[int]TreeNode
     */
    protected $children = null;

    /**
     * This method is the contructor of the class.
     *
     * @param TreeNode $node
     * @return void
     */
    public function __construct(TreeNode $node)
    {
        $this->children = $node->getChildren();
    }

    /**
     * This method returns the current child.
     *
     * @return TreeNode
     */
    public function current()
    {
        return current($this->children);
    }

    /**
     * This method returns the index of the current child.
     *
     * @return int
     */
    public function key()
    {
        return key($this->children);
    }

    /**
     * This method moves the pointer to the next child.
     *
     * @return void
     */
    public function next()
    {
        next($this->children);
    }

    /**
     * This method moves the pointer to the beginning of the child array.
     *
     * @return void
     */
    public function rewind()
    {
        reset($this->children);
    }

    /**
     * This method verifies if the the current pointer position is still valid
     *
     * @return boolean true if the current pointer position is still valid, else false
     */
    public function valid()
    {
        return (current($this->children) !== FALSE);
    }
}
?>