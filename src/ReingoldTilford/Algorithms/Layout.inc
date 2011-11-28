<?php

namespace ReingoldTilford\Algorithms;

use ReingoldTilford\Styles\TreeStyle;
use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class encapsulates the layout of a collection of (tree) nodes.
 */
class Layout implements \Iterator
{
    /**
     * the collection of coorindates
     *
     * @var \SplObjectStorage
     */
    private $coordinates = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param \SplObjectStorage $coordinates the collection of coorindates
     */
    public function __construct(\SplObjectStorage $coordinates)
    {
        $this->coordinates = $coordinates;
    }

    /**
     * This method normalizes the layout, such that no negative coordinates remain, i.e. the lowest x-coordinate will be 0 and will denote the left-most leaf of the tree.
     *
     * @return Layout $this
     */
    public function normalize()
    {
        $minXPoint = null;
        foreach($this->coordinates as $node)
        {
            $currentPoint = $this->coordinates[$node];

            if($minXPoint === null || $currentPoint->x < $minXPoint->x)
                $minXPoint = $currentPoint;
        }

        if($minXPoint !== null)
        {
            foreach($this->coordinates as $node)
                $this->coordinates[$node] = $this->coordinates[$node]->moveBy(new Dimension(-$minXPoint->x, 0));
        }

        return $this;
    }

    /**
     * This method gets the dimensions of the layout.
     *
     * @param TreeStyle $treeStyle the tree style which is used for calculating the dimensions
     * @return Dimension the dimensions of the layout
     */
    public function getDimension(TreeStyle $treeStyle)
    {
        $maxX = 0;
        $maxY = 0;

        foreach($this->coordinates as $node)
        {
            $currentX = $this->getXCoordinate($node, $treeStyle);

            $currentY = $this->getYCoordinate($node, $treeStyle);

            $maxX = max($maxX, $currentX + ($treeStyle->shapeStyle->width + ($treeStyle->shapeStyle->border->width * 1) + $treeStyle->xPadding));
            $maxY = max($maxY, $currentY + ($treeStyle->shapeStyle->height + ($treeStyle->shapeStyle->border->width * 1) + $treeStyle->yPadding));
        }

        return new Dimension($maxX, $maxY);
    }

    /**
     * This method returns the position of a given tree node with respect to a given tree style.
     *
     * @param object $node the node for which to get the position
     * @param TreeStyle $treeStyle the tree style to respect
     * @return Point the position of the node
     */
    public function getPosition($node, TreeStyle $treeStyle)
    {
        return new Point($this->getXCoordinate($node, $treeStyle), $this->getYCoordinate($node, $treeStyle));
    }

    /**
     * This method returns the x-coordinate of a given tree node with respect to a given tree style.
     *
     * @param object $node the node for which to get the position
     * @param TreeStyle $treeStyle the tree style to respect
     * @return int the x-coordinate of a given tree node
     */
    public function getXCoordinate($node, TreeStyle $treeStyle)
    {
        $factor = $treeStyle->shapeStyle->width + $treeStyle->xOffset + ($treeStyle->shapeStyle->border->width * 2);

        return round($treeStyle->xPadding
                + $treeStyle->shapeStyle->border->width
                + ($this->coordinates[$node]->x * $factor), 0);
    }

    /**
     * This method returns the y-coordinate of a given tree node with respect to a given tree style.
     *
     * @param object $node the node for which to get the position
     * @param TreeStyle $treeStyle the tree style to respect
     * @return int the y-coordinate of a given tree node
     */
    public function getYCoordinate($node, TreeStyle $treeStyle)
    {
        $factor = $treeStyle->shapeStyle->height + $treeStyle->yOffset + ($treeStyle->shapeStyle->border->width * 2);

        return round($treeStyle->xPadding
                + $treeStyle->shapeStyle->border->width
                + ($this->coordinates[$node]->y * $factor), 0);
    }

    /**
     *
     */
    public function current()
    {
        return $this->coordinates->current();
    }

    /**
     *
     */
    public function key()
    {
        return $this->coordinates->key();
    }

    /**
     *
     */
    public function next()
    {
        return $this->coordinates->next();
    }

    /**
     *
     */
    public function rewind()
    {
        return $this->coordinates->rewind();
    }

    /**
     *
     */
    public function valid()
    {
        return $this->coordinates->valid();
    }
}