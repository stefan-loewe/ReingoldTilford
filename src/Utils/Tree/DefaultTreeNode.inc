<?php

namespace Utils\Tree;

/**
 * This class encapsulates a simple node in a tree.
 */
class DefaultTreeNode
{
    /**
     * the data item of the node
     *
     * @var mixed
     */
    protected $data     = null;

    /**
     * the parent of the node
     *
     * @var TreeNode
     */
    protected $parent   = null;

    /**
     * the children of a node
     *
     * @var array[int]TreeNode
     */
    protected $children = array();

    /**
     * This method is the constructor of the class.
     *
     * @param mixed $data the data item the nde encapsulates
     * @return void
     */
    public function __construct($data)
    {
        $this->data = $data;
    }

    /**
     * This method returns the parent node of the node.
     *
     * @return TreeNode
     */
    public function getParent()
    {
        return $this->parent;
    }

    /**
     * This method returns the child at the given index, or null if it does not exist.
     *
     * @param int $index the index of the child to get
     * @return DefaultTreeNode the child at the given index or null if it does not exist
     */
    public function getChild($index)
    {
        if(isset($this->children[$index]) && $this->children[$index] != null)
            return $this->children[$index];

        else
            return null;
    }

    /**
     * This method appends a node to the current node.
     *
     * @param TreeNode $child
     * @return TreeNode $this
     */
    public function appendChild(self $child)
    {
        $this->children[] = $child;

        $child->parent     = $this;

        return $this;
    }
}