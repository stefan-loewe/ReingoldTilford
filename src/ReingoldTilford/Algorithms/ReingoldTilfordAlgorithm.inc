<?php

namespace ReingoldTilford\Algorithms;

use Utils\Geom\Point;
use Utils\Geom\Dimension;
use ReingoldTilford\Algorithms\Layout;
use ReingoldTilford\Models\ITreeModel;
use ReingoldTilford\Styles\TreeStyle;

/**
 * This clas implements the Reingold-Tilford tree drawing algorithm. The alorithm works in O(n), where n is the number of nodes in the tree.
 */
class ReingoldTilfordAlgorithm
{
    /**
     * one child only makes an only child degree
     */
    private static $ONLY_CHILD_DEGREE   = 1;

    /**
     * two children make an binary degree
     */
    private static $BINARY_DEGREE       = 2;

    /**
     * the tree model the algorithm uses
     *
     * @var ITreeModel
     */
    private $model                  = null;

    /**
     * the root of the (sub)tree to be printed
     *
     * @var TreeNode
     */
    private $root                   = null;

    /**
     * the minimal horizontal distance of two nodes
     *
     * @var int
     */
    private $minDistance            = 1;

    /**
     * the left outline of the nodes
     *
     * @var SplObjectStorage
     */
    private $leftOutline            = null;

    /**
     * the right outline of the nodes
     *
     * @var SplObjectStorage
     */
    private $rightOutline           = null;

    /**
     * the current right outline (i.e. right outline of the current left subtree)
     *
     * @var array
     */
    private $currentRightOutline    = null;

    /**
     * the left threads
     *
     * @var SplObjectStorage
     */
    private $leftThread             = null;

    /**
     * the right threads
     *
     * @var SplObjectStorage
     */
    private $rightThread            = null;

    /**
     * the x-offsets of the nodes
     *
     * @var SplObjectStorage
     */
    private $offsets                = null;

    /**
     * hash table that maps a node to its parent
     *
     * @var SplObjectStorage
     */
    private $parents                = null;

    /**
     * hash table that maps a node to its depth
     *
     * @var SplObjectStorage
     */
    private $depths                 = null;

    /**
     * the orientation variant of the tree
     *
     * @var string
     */
    private $orientation            = TreeStyle::CENTERED;

    /**
     * a reference to the children of the currently visited node (cached for performance reasons)
     *
     * @var array[int]TreeNode
     */
    private $children               = null;

    /**
     * the number of children of the currently visited node (cached for performance reasons)
     *
     * @var int
     */
    private $nodeDegree             = null;

    /**
     * This method acts as the constructor of the class.
     *
     */
    public function __construct()
    {
    }

    /**
     * This method (re)initializes the internal data structures, so an object of this class can be used for several trees to be drawn.
     *
     * @return void
     */
    private function initialize()
    {
        $this->leftOutline  = new \SplObjectStorage();
        $this->rightOutline = new \SplObjectStorage();

        $this->leftThread   = new \SplObjectStorage();
        $this->rightThread  = new \SplObjectStorage();

        $this->offsets      = new \SplObjectStorage();

        $this->initializeMetaData();
    }

    /**
     * This method calculates the offsets of a node.
     *
     * This is the main method of the algorithm.
     *
     * @param object $node
     */
    private function calculatesOffsets($node)
    {
        $this->children     = $this->getChildren($node);
        $this->nodeDegree   = $this->getDegree($node);

        // initialize containers for the left and right outlines of the children ...
        $leftOutlines       = array();
        $rightOutlines      = array();

        // ... and fill them
        foreach($this->children as $index => $child)
        {
            $leftOutlines[$index]   = $this->leftOutline[$child];
            $rightOutlines[$index]  = $this->rightOutline[$child];
        }

        // initialize the current right outline (i.e. right outline of left subtree) as empty ...
        $this->currentRightOutline = array();
        // ... unless there are children, then the current right outline of the current tree is the right outline of the very first child of the node
        if($this->nodeDegree > 0)
            $this->currentRightOutline = current($rightOutlines);

        // calculate the shifts off each subtree ...
        $subTreeShifts = $this->calculateSubTreeShifts($node, $leftOutlines, $rightOutlines);

        // ... set the offsets of the nodes ...
        $this->setNodeOffsets($node, $subTreeShifts);

        // ... and update the outlines
        $this->updateOutlines($node, $leftOutlines);
    }

    /**
     * This method returns the subtree shifts of the children of the current node.
     *
     * @param object $node the current node that is traversed
     * @param array[int][int]TreeNode $leftOutlines the left outlines of the node
     * @param array[int][int]TreeNode $rightOutlines the right outlines of the node
     * @return array[int]int
     */
    private function calculateSubTreeShifts($node, $leftOutlines, $rightOutlines)
    {
        // initialize the subtree shifts, with the first subtree always having no shift
        $subTreeShifts = array(0 => 0);
        // iterate over the children, starting with second one, and compare the current right outline
        // with the left outline of the child of the current iteration
        $firstChildPassed = FALSE;

        foreach($this->children as $i => $child)
        {
            if(!$firstChildPassed)
            {
                $firstChildPassed = TRUE;
                continue;
            }

            $subTreeShifts[$i] = $this->minDistance;

            if(($length = min(count($this->currentRightOutline), count($leftOutlines[$i]))) > 1)
            {
                $leftSubTreeOffset  = 0;
                $rightSubTreeOffset = 0;
                // iterate thru the outlines, upto the length of the shorter one
                for($j = 1; $j < $length; $j++)
                {
                    // get the current node's offsets, and accumulate them to the offsets of their predecessors in the outlines
                    $leftSubTreeOffset = $leftSubTreeOffset + $this->offsets[$this->currentRightOutline[$j]] - ($this->rightThread->offsetExists($this->currentRightOutline[$j]) ? $this->rightThread[$this->currentRightOutline[$j]] : 0);
                    $rightSubTreeOffset = $rightSubTreeOffset + $this->offsets[$leftOutlines[$i][$j]] + ($this->leftThread->offsetExists($leftOutlines[$i][$j]) ? $this->leftThread[$leftOutlines[$i][$j]] : 0);

                    // compare the right outline of the left subtree to the left outline of the right subtree, and get the maximum over the outline
                    $subTreeShifts[$i] = max(max($leftSubTreeOffset - $rightSubTreeOffset + $this->minDistance, 0), $subTreeShifts[$i]);
                }
            }

            // set a temporary offset for the current child, ...
            $this->offsets[$child] = array_sum($subTreeShifts);

            // ... build the new right outline for the upcoming iteration, which is the right outline of the following child
            // plus the rest of the nodes of the current outline in case the current outline is longer ...
            for($index = 0, $nextRightLength = count($rightOutlines[$i]); $index < $nextRightLength; $index++)
                $this->currentRightOutline[$index] = $rightOutlines[$i][$index];

            // ... and add a thread in case the next outline was shorter than the current one
            if(count($this->currentRightOutline) > $nextRightLength)
            {
                $threadSource = $rightOutlines[$i][$nextRightLength - 1];
                $threadTarget = $this->parents[$this->currentRightOutline[$nextRightLength]];

                $offsetRight  = 0;
                $offsetLeft   = 0;
                while($threadSource !== $node)
                {
                    $offsetRight    = $offsetRight + ($this->offsets->offsetExists($threadSource) ? $this->offsets[$threadSource] : 0);
                    $offsetLeft     = $offsetLeft + ($this->offsets->offsetExists($threadTarget) ? $this->offsets[$threadTarget] : 0);
                    $threadTarget   = $this->parents[$threadTarget];
                    $threadSource   = $this->parents[$threadSource];
                }
                $this->rightThread[$this->currentRightOutline[$nextRightLength]] = $offsetRight - $offsetLeft;
            }
        }

        return $subTreeShifts;
    }

    /**
     * This method sets the offsets of the children of the current node.
     *
     * @param object $node the current node that is traversed
     * @param array[int]double $subTreeShifts the shifts of the subtrees
     * @return void
     */
    private function setNodeOffsets($node, $subTreeShifts)
    {
        if($this->model->isLeaf($node))
            $this->offsets[$node] = 0;

        else
        {
            // place an only child right beneath the father
            if($this->nodeDegree == self::$ONLY_CHILD_DEGREE)
                $this->offsets[$this->children[0]] = 0;

            else
            {
                // find the node whose final shift would be closest to 0 (0 = directly under father), and set that shift as the value of offCenter ...
                $offCenter = null;
                if($this->orientation == TreeStyle::CENTERED && $this->nodeDegree > self::$BINARY_DEGREE)
                {
                    $shift = -1 * intval((array_sum($subTreeShifts)) / 2);
                    foreach($this->children as $cnt => $child)
                    {
                        $shift = $shift + $subTreeShifts[$cnt];
                        // "<=", not "<" here, as it can happen that the very first node (of maybe 3) fulfills this condition, as well as the 2nd one,
                        // as both have the same distance to the center. Then, with "<", the 2nd one (the more suitable one) would get blocked.
                        if($offCenter === null || abs($shift) <= abs($offCenter))
                            $offCenter = $shift;
                    }
                }

                // ... and adapt the shifts accordingly, so the the child closest to their father actually is placed directly under its father (offset 0)
                $shift = -1 * intval((array_sum($subTreeShifts)) / 2) - $offCenter;
                if($this->orientation == TreeStyle::LEFT_BOUND)
                    $shift = 0;
                else if($this->orientation == TreeStyle::RIGHT_BOUND)
                    $shift = -1 * intval((array_sum($subTreeShifts))) - $offCenter;

                // center children on binary subtree
                if($this->orientation == TreeStyle::CENTERED && $this->nodeDegree == self::$BINARY_DEGREE)
                {
                    $this->offsets[$this->children[0]] = -$subTreeShifts[1] / 2;
                    $this->offsets[$this->children[1]] = $subTreeShifts[1] / 2;
                }
                // align children according to their shifts
                else
                {
                    foreach($this->children as $cnt => $child)
                    {
                        $shift = $shift + (array_key_exists($cnt, $subTreeShifts) ? $subTreeShifts[$cnt] : 0);
                        $this->offsets[$child] = $shift;
                    }
                }
            }
        }
    }

    /**
     * This method updates the left and the right outline of the current node.
     *
     * @param object $node the current node that is traversed
     * @param array[int][int]TreeNode $leftOutlines the left outlines of the node
     * @return void
     */
    private function updateOutlines($node, $leftOutlines)
    {
        // both the left and right outline of the current node contain at least the current node itself
        $leftOutline    = array($node);
        $rightOutline   = array($node);

        // if the node is a leaf, both the right and left outline consist of this leaf only
        if($this->model->isLeaf($node))
        {
            $this->leftOutline[$node]   = $leftOutline;
            $this->rightOutline[$node]  = $rightOutline;
        }

        else
        {
            // set the right outline, being up to date anyway ...
            $this->rightOutline[$node] = array_merge($rightOutline, $this->currentRightOutline);

            // .. and update and set the left outline ...
            for($i = 1; $i < $this->nodeDegree; $i++)
            {
                // ... initially consisting of the left outline of the left most child, and being incremented by the left
                // outline of any succeeding child, if its outline is longer than the current combined left outline
                if(($leftLength = count($leftOutlines[0])) < ($nextOutlineLength = count($leftOutlines[$i])))
                {
                    for($j = $leftLength; $j < $nextOutlineLength; $j++)
                    {
                        $leftOutlines[0][$j] = $leftOutlines[$i][$j];
                        // add a thread
                        if($j == $leftLength)
                        {
                            // the source is the node at the end of the shorter left outline
                            $threadSource = $leftOutlines[0][$j - 1];
                            // the target is the father of the node that is one level deeper as the thread source on the longer left outline,
                            // so the father of that node is on the same level as the thread source
                            $threadTarget = $this->parents[$leftOutlines[$i][$j]];

                            // walk from the source and the target to the current node (nearest common ancestor), adding up their offsets
                            $offsetSource = 0;
                            $offsetTarget = 0;
                            while($threadSource !== $node)
                            {
                                $offsetSource = $offsetSource + $this->offsets[$threadSource];
                                $offsetTarget = $offsetTarget + $this->offsets[$threadTarget];

                                //echo '<br>source: moving from '.$threadSource->getData();
                                $threadSource = $this->parents[$threadSource];
                                //echo 'to '.$threadSource->getData().' with combined offset of '.$offsetSource;
                                //echo '<br>target: moving from '.$threadTarget->getData();
                                $threadTarget = $this->parents[$threadTarget];
                                //echo 'to '.$threadTarget->getData().' with combined offset of '.$offsetTarget;
                            }
                            //echo '<br>RESULT '.($offsetTarget - $offsetSource);

                            $this->leftThread[$leftOutlines[$i][$j]] = $offsetTarget - $offsetSource;
                        }
                    }
                }
            }

            $this->leftOutline[$node] = array_merge($leftOutline, $leftOutlines[0]);
        }
    }

    /**
     * This method returns a Layout for a given ITreeModel.
     *
     * @param ITreeModel $model the tree model to get a layout for
     * @param type $orientation the orientation of the children of a node, either one of TreeStyle::CENTERED, TreeStyle::LEFT, TreeStyle::RIGHT
     * @return Layout the layout of the given tree model
     */
    public function getLayout(ITreeModel $model, $orientation = TreeStyle::CENTERED)
    {
        $this->model = $model;

        // set the root ...
        $this->root = $model->getRoot();

        // .. the orientation ...
        $this->orientation = $orientation;

        // ... and (re)initialize the internal data structures
        $this->initialize();

        // get the relative shifts
        foreach($this->getPostOrderEnumeration() as $node)
            $this->calculatesOffsets($node);

        // calculate the absolute offsets from the relative shifts
        $offsets = new \SplObjectStorage();
        $offsets[$this->root] = new Point(0, 0);
        foreach($this->getPreOrderEnumeration() as $node)
        {
            if($node !== $this->root)
                $offsets[$node] = $offsets[$this->parents[$node]]->moveBy(new Dimension($this->offsets[$node], 1));
        }

        return new Layout($offsets);
    }

    /**
     * This helper method initializes the meta data for use in the algorithm
     */
    private function initializeMetaData()
    {
        $todo           = new \SplStack();

        $this->parents  = new \SplObjectStorage();
        $this->depths   = new \SplObjectStorage();

        $todo->push($root = $this->model->getRoot());

        $this->parents[$root] = null;
        $this->depths[$root]  = 0;

        while($todo->count() > 0)
        {
            $currentNode = $todo->pop();

            for($i = $this->model->getChildCount($currentNode) - 1; $i >= 0; $i--)
            {
                $todo->push($child = $this->model->getChild($currentNode, $i));

                $this->parents[$child]  = $currentNode;
                $this->depths[$child]   = $this->depths[$currentNode] + 1;
            }
        }
    }

    /**
     * This helper method returns the pre-order enumaration of the tree model.
     *
     * @return \SplDoublyLinkedList the pre-order enumaration of the tree model
     */
    private function getPreOrderEnumeration()
    {
        $todo       = new \SplStack();
        $enum       = new \SplDoublyLinkedList();

        $todo->push($this->model->getRoot());

        while($todo->count() > 0)
        {
            $enum->push($currentNode = $todo->pop());

            for($i = $this->model->getChildCount($currentNode) - 1; $i >= 0; $i--)
                $todo->push($this->model->getChild($currentNode, $i));
        }

        return $enum;
    }

    /**
     * This helper method returns the post-order enumaration of the tree model.
     *
     * @return \SplDoublyLinkedList the post-order enumaration of the tree model
     */
    private function getPostOrderEnumeration()
    {
        $todo       = new \SplStack();
        $visited    = new \SplObjectStorage();
        $enum       = new \SplDoublyLinkedList();

        $todo->push($this->model->getRoot());

        while($todo->count() > 0)
        {
            $currentNode = $todo->pop();

            if(!$visited->contains($currentNode) && !$this->model->isLeaf($currentNode))
            {
                $todo->push($currentNode);

                for($i = $this->model->getChildCount($currentNode) - 1; $i >= 0; $i--)
                    $todo->push($this->model->getChild($currentNode, $i));
            }

            if($visited->contains($currentNode) || $this->model->isLeaf($currentNode))
                $enum->push($currentNode);

            $visited[$currentNode] = TRUE;
        }

        return $enum;
    }

    /**
     * This helper method returns the child nodes of a given node.
     *
     * @param object $node the node for which to get its child nodes
     * @return \ArrayObject of child nodes
     */
    private function getChildren($node)
    {
        $children = new \ArrayObject();

        for($i = 0; $i < $this->model->getChildCount($node); $i++)
            $children[] = $this->model->getChild($node, $i);

        return $children;
    }

    /**
     * This helper method gets the degree of a given node.
     *
     * @param object $node the node of which the degree has to be calculated
     * @return int the degree of the node
     */
    private function getDegree($node)
    {
        return $this->model->getChildCount($node);
    }
}