<?php

namespace Utils\Tree;

class DotTreeNode extends TreeNode
{
    private $id = null;

    public function __construct($id, $label)
    {
        parent::__construct($label);

        $this->id = $id;

//echo "\ncreating DotTreeNode with id ".$id." and label ".$label;
    }

    public function getID()
    {
        return $this->id;
    }
}
