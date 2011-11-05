<?php

namespace Utils\Tree;

/**
 * This class encapsulates a node in a tree.
 */
class TreeNode implements \IteratorAggregate
{
    /**
     * the data item of the node
     *
     * @var mixed
     */
    protected $data        = null;

    /**
     * the parent of the node
     *
     * @var TreeNode
     */
    protected $parent    = null;

    /**
     * the children of a node
     *
     * @var array[int]TreeNode
     */
    protected $children    = array();

    /**
     * the number of levels to the root
     *
     * @var int
     */
    protected $depth    = 0;

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
     * This method populates a tree node randomly, and is used for testing purposes only, and returns the colection of inserted node as array.
     *
     * @param int $size the size of the tree including the current node
     * @param int $maxDegree the maximal degree of the nodes, if 0 is passed, the degree is unlimited
     * @return array[string]TreeNode
     */
    public function populateRandomly($size = 100, $maxDegree = 0)
    {
        // maxDegree must be greater or equal to 0
        $maxDegree    = max(0, $maxDegree);

        // add the root to the index ...
        $index[]    = $this;

        // ... and add children iteratively in a random way
        for($i = 1; $i < $size; $i++)
        {
            // pick a random parent among the nodes already inserted ...
            $parentIndex = array_rand($index);
            /*
             * for more height: if the parent is the root, try again
            if($parentIndex === 0)
                $parentIndex = array_rand($index);*/
            $parent = $index[$parentIndex];

            // ... and add it to the parent if the parent has not yet at least $maxDegree children, ...
            if($maxDegree == 0 || $parent->getDegree() < $maxDegree)
            {
                $child = new TreeNode($i);
                $parent->appendChild($child);
                $index[] = $child;
            }
            // ... otherwise, revert and try to pick a new one
            else
                $i = $i - 1;
        }
        return $index;
    }

    /**
     * This method returns the data element of the node.
     *
     * @return mixed
     */
    public function getData()
    {
        return $this->data;
    }

    /**
     * This method sets the data element of the node.
     *
     * @param mixed $data the new data element of the node
     */
    public function setData($data)
    {
        $this->data = $data;
    }

    /**
     * This method returns the depth of the node.
     *
     * @return int
     */
    public function getDepth()
    {
        return $this->depth;
    }

    /**
     * This method returns the height of the node.
     *
     * @return int
     */
    public function getHeight()
    {
        $height = 0;

        foreach(new \RecursiveIteratorIterator($this->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $node)
            $height = max($height, $node->depth - $this->depth);

        return $height;
    }

    /**
     * This method returns the root node of the current node.
     *
     * @return TreeNode
     */
    public function getRoot()
    {
        $currentNode = $this;

        while($currentNode->parent != null)
            $currentNode = $currentNode->parent;

        return $currentNode;
    }

    /**
     * This method returns the first node found with the given data item.
     *
     * @param mixed $data the data item to seach for
     * @return TreeNode the first node that has the given data item
     */
    public function getNodeByData($data)
    {
        if($data == $this->data)
            return $this;

        foreach(new \RecursiveIteratorIterator($this->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $node)
        {
            if($data == $node->data)
                return $node;
        }

        return null;
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
     * This method returns all parent nodes of the node.
     *
     * @return \ArrayObject<TreeNode>
     */
    public function getParents()
    {
        $parents = new \ArrayObject();

        $currentNode = $this;

        while(($currentNode = $currentNode->parent) != null)
            $parents[] = $currentNode;

        return $parents;
    }

    /**
     * This method determines if the current node is an ancestor of another one
     *
     * @param TreeNode $descendant
     * @return boolean true if the current node is an ancestor
     */
    public function isAncestorOf(TreeNode $descendant)
    {
        while($descendant != null)
        {
            if($descendant === $this)
                return TRUE;

            $descendant = $descendant->getParent();
        }

        return FALSE;
    }

    /**
     * This method determines if the current node is a descendant of another one
     *
     * @param TreeNode $ancestor
     * @return boolean true if the current node is a descendant
     */
    public function isDescendantOf(TreeNode $ancestor)
    {
        return $ancestor->isAncestorOf($this);
    }

    /**
     * This method returns the degree of a node.
     *
     * @return int
     */
    public function getDegree()
    {
        return count($this->getChildren());
    }

    /**
     * This method gets the number of nodes in the whole subtree, including the current node.
     *
     * @return int
     */
    public function getSize()
    {
        $count = 1;

        foreach(new \RecursiveIteratorIterator($this->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $node)
            $count++;

        return $count;
    }

    /**
     * This method returns the first child of the node.
     *
     * @return TreeNode
     * @throws RuntimeException if the node has no children
     */
    public function getFirstChild()
    {
        foreach($this->children as $child)
            return $child;

        throw new \RuntimeException('The node has no children!');
    }

    /**
     * This method returns the last child of the node.
     *
     * @return TreeNode
     * @throws RuntimeException if the node has no children
     */
    public function getLastChild()
    {
        if(($lastChild = end($this->children)) === FALSE)
            throw new \RuntimeException('The node has no children!');
        else
            return $lastChild;
    }

    /**
     * This method returns the child at the given index or null if no child exists at that given index.
     *
     * @param int $index the index of the child
     * @return TreeNode the child at the given index
     * @throws RuntimeException if no child is at the pointed index but the index was correct
     * @throws OutOfBoundsException if the passed index exceeds the number of children
     */
    public function getChildAtIndex($index)
    {
        if($this->indexIsWithinBounds($index))
        {
            // ... and does it point to a correct entry?
            if(isset($this->children[$index]) && $this->children[$index] != null)
                return $this->children[$index];

            // if not, raise a runtime exception (this should never happen unless appendChild or removeChild are erroneous)
            else
                throw new \RuntimeException('There is no child set at the index "'.$index.'" of node "'.$this->id.'"!');
        }
        else
            throw new \OutOfBoundsException('The index "'.$index.'" is out of bounds!');

        return null;
    }

    /**
     * This method checks if the child index is within bounds.
     *
     * This method serves as hook for being overloaded by a sub-class, to implement its own rule for checking allowed indices.
     *
     * @param $index int
     * @return boolean true, if the child index is within bounds, else false
     */
    protected function indexIsWithinBounds($index)
    {
        return 0 <= $index && $index < $this->getDegree();
    }

    /**
     * This method returns the position of the child in the list of children of this node, -1 if the child is not a son of this node.
     *
     * @param TreeNode $child
     * @return int
     * @throws UnexpectedValueException if the passed node is not a child of the current node
     */
    public function getIndexOfChild(TreeNode $child)
    {
        $index = -1;

        foreach($this->children as $cnt => $currentChild)
        {
            if($currentChild == $child)
                $index = $cnt;
        }

        if($index == -1)
            throw new \UnexpectedValueException('The passed node is not a child "'.$child->id.'" of the current node!');

        return $index;
    }

    /**
     * This method returns a node by a path, e.g. passing array(5, 3, 0) would return the 1st child of the 3rd child of the 5th child of the node.
     *
     * @param array $path an array of integers representing the path in form of child indices
     * @return TreeNode the node at the end of the path or null if not existent
     */
    public function getNodeByPath(array $path)
    {
        $currentNode = $this;

        foreach($path as $index)
            $currentNode = $currentNode->getChildAtIndex($index);

        return $currentNode;
    }

    /**
     * This method returns the array of children.
     *
     * @return array[int]TreeNode
     */
    public function getChildren()
    {
        return $this->children;
    }

    /**
     * This method get the number of nodes at each level as a hash.
     *
     * @return array
     */
    public function getNodeCountsPerLevel()
    {
        $currentNodeDepth = $this->getDepth();

        $counts = array();

        $counts[0] = 1;

        foreach(new \RecursiveIteratorIterator($this->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $node)
            !isset($counts[$depth = $node->getDepth() - $currentNodeDepth]) ? $counts[$depth] = 1 : $counts[$depth]++;

        return $counts;
    }

    /**
     * This method get the number of neighbours of a node, which is the same as the number of children in case called for the root, and is the same as the number of children plus one for all other nodes.
     *
     * @return int
     */
    public function getNeighbourCount()
    {
        return $this->getDegree() + ($this->parent === null ? 0 : 1);
    }

    /**
     * This method returns if a node is a leaf.
     *
     * @return boolean
     */
    public function isLeaf()
    {
        return $this->getDegree() == 0;
    }

    /**
     * This method returns the number of leafs of the current node.
     *
     * @return int
     */
    public function getLeafCount()
    {
        $leafCount = 0;
        foreach($this->children as $child)
        {
            if($child->isLeaf())
                $leafCount++;
        }
        return $leafCount;
    }

    /**
     * This method returns the number of leafs in the subtree of the current node.
     *
     * @return int
     */
    public function getSubTreeLeafCount()
    {
        $leafCount = 0;
        foreach(new \RecursiveIteratorIterator($this->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $child)
        {
            if($child->isLeaf())
                $leafCount++;
        }
        return $leafCount;
    }

    /**
     * This method appends a node to the current node, and removes it from its previous parent if it had one, and returns the current node.
     *
     * @param TreeNode $child
     * @return TreeNode
     * @throws UnexpectedValueException if an ancestor of the current node is inserted
     */
    public function appendChild(self $child)
    {
        // an ancestor of the current node cannot be inserted
        if($child->isAncestorOf($this))
        {
            throw new \UnexpectedValueException();
        }

        // if the child already has a parent, remove it from its current parent first ...
        if(($currentParent = $child->getParent()) != null)
            $currentParent->removeChild($child);

        // ... then insert it to the parent's children array ...
        $this->insertChild($child);

        // .. set the parent ...
        $child->parent        = $this;

        // ... and update the child's depth ...
        $depthDiff            = $this->depth + 1 - $child->depth;
        $child->depth        = $child->depth + $depthDiff;

        // ... and the depth of the nodes in the subtree of the child
        foreach(new \RecursiveIteratorIterator($child->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $subtreeChild)
            $subtreeChild->depth += $depthDiff;

        return $this;
    }

    /**
     * This method inserts a child into the child array.
     *
     * This method serves as hook for being overloaded by a sub-class, to implement its own rule to insert children.
     *
     * @param TreeNode $child
     * @return void
     */
    protected function insertChild(self $child)
    {
        $this->children[] = $child;
    }

    /**
     * This method removes a child from the current node and returns the current node.
     *
     * @param TreeNode $child
     * @return TreeNode
     */
    public function removeChild(self $child)
    {
        $child->parent = null;

        $deleted = FALSE;

        // search the child ...
        foreach($this->children as $cnt => $kid)
        {
            // ... and remove it from the child list once it is found
            if($deleted = ($child == $kid))
            {
                unset($this->children[$cnt]);
                break;
            }
        }

        // if a child was deleted re-index the child-array ...
        if($deleted)
            $this->reIndexChildArray();
        // ... otherwise, throw an exception
        else
            throw new \UnexpectedValueException();

        return $this;
    }

    /**
     * This method resets the indices of the child array after a node was deleted.
     *
     * @return void
     */
    protected function reIndexChildArray()
    {
        $this->children = array_values($this->children);
    }

    /**
     * This mehod returns an interator to iterate through the children of a node.
     *
     * @return TreeNodeIterator
     */
    public function getIterator()
    {
        return new TreeNodeIterator($this);
    }

    /**
     * This mehod returns an interator to iterate through the whole subtree of a node.
     *
     * @return RecursiveTreeNodeIterator
     */
    public function getRecursiveIterator()
    {
        return new RecursiveTreeNodeIterator($this);
    }

    /**
     * This method exports the tree as array.
     *
     * @param boolean $nested if true, the tree will be exported as nested array, else the array will be flat
     * @return array[string][string]mixed
     */
    public function toArray($nested = TRUE)
    {
        // either build a nested array structure ...
        if($nested)
        {
            $children = array();
            foreach($this->children as $child)
                $children[] = $child->toArray();

            $node = array(
                            'data'        => $this->data,
                            'children'    => $children
                        );
        }
        // ... or build a flat array
        else
        {
            $node[$this->id] = array('data' => $this->data);
            foreach(new \RecursiveIteratorIterator($this->getRecursiveIterator(), \RecursiveIteratorIterator::SELF_FIRST) as $child)
            {
                $node[] = array(
                                    'data'        => $child->data,
                                    'parent'    => $child->id
                                );
            }
        }
        return $node;
    }

    /**
     * This method returns the tree as a JSON string.
     *
     * @return string the JSON representation of the tree
     */
    public function toJSON()
    {
        return json_encode($this->toArray(FALSE));
    }

    /**
     * This method returns the node as string.
     *
     * @return string
     */
    public function __toString()
    {
        //return 'level '.$this->depth.': id = '.$this->id.($this->parent === null ? ' [ROOT]' : ' [parent = '.$this->parent->id.']');
        return 'level '.$this->depth.': data = '.$this->data;
    }

    /**
     * This method returns the node as string.
     *
     * @return string
     */
    public function toStringRec()
    {
        $result = '<node>'."\n";
        $result = $result.'<id>'.$this->data.'</id>'."\n";
        foreach($this->children as $child)
            $result = $result.$child->toStringRec();
        $result = $result.'</node>'."\n";
        return $result;
    }
}
?>
