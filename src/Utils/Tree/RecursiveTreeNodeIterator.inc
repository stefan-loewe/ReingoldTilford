<?php

namespace Utils\Tree;

/**
 * This class implements an recursive iterator that allows for recursive visiting of tree nodes (@see \Lib\TreeNode)
 */
class RecursiveTreeNodeIterator extends TreeNodeIterator implements \RecursiveIterator
{
    /**
     * This method is the contructor of the class.
     *
     * @param TreeNode $node
     * @return void
     */
    public function __construct(TreeNode $node)
    {
        parent::__construct($node);
    }

    /**
     * This method determines if the iterator has children to work on.
     *
     * @return boolean true, if the iterator has children to work on, else false
     */
    public function hasChildren()
    {
        return (count($this->children) > 0);
    }

    /**
     * This method returns a new RecursiveTreeNodeIterator to work on the current child
     *
     * @return RecursiveTreeNodeIterator
     */
    public function getChildren()
    {
        return new RecursiveTreeNodeIterator(current($this->children));
    }
}
?>