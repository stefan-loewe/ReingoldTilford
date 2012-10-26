<?php

namespace ws\loewe\ReingoldTilford\Plotters;

use \ws\loewe\ReingoldTilford\Algorithms\Layout;
use \ws\loewe\ReingoldTilford\Models\ITreeModel;
use \ws\loewe\ReingoldTilford\Styles\TreeStyle;
use \ws\loewe\Utils\Graphics2D\Shapes\ShapeFactory;
use \ws\loewe\Utils\Graphics2D\Shapes\EdgeFactory;
use \ws\loewe\Utils\Graphics2D\Shapes\TextFactory;
use \ws\loewe\Utils\Graphics2D\DrawingPanes\DrawingPane;

/**
 * This class can plot the layout returned by the Reingolf-Tilford tree drawing algorithm.
 */
class Plotter
{
    /**
     * the tree model
     *
     * @var ITreeModel
     */
    private $model          = null;

    /**
     * the layout
     *
     * @var Layout
     */
    private $layout         = null;

    /**
     * the tree style
     *
     * @var TreeStyle
     */
    private $treeStyle      = null;

    /**
     * the factory for creating shapes
     *
     * @var ShapeFactory
     */
    private $shapeFactory   = null;

    /**
     * the factory for creating edges
     *
     * @var EdgeFactory
     */
    private $edgeFactory    = null;

    /**
     * the factory for creating texts
     *
     * @var TextFactory
     */
    private $textFactory    = null;

    /**
     * the mapping from nodes to shapes
     *
     * @var \SplObjectStorage
     */
    private $shapeMap       = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param ITreeModel $model the tree model
     * @param Layout $layout the layout
     * @param ShapeFactory $shapeFactory the factory for creating shapes
     * @param EdgeFactory $edgeFactory the factory for creating edges
     * @param TextFactory $textFactory the factory for creating texts
     */
    public function __construct(ITreeModel $model, Layout $layout, ShapeFactory $shapeFactory = null, EdgeFactory $edgeFactory = null, TextFactory $textFactory = null)
    {
        $this->model        = $model;

        $this->layout       = $layout;

        $this->shapeFactory = $shapeFactory == null ? new ShapeFactory() : $shapeFactory;

        $this->edgeFactory  = $edgeFactory == null ? new EdgeFactory() : $edgeFactory;

        $this->textFactory  = $textFactory == null ? new TextFactory() : $textFactory;

        $this->shapeMap     = new \SplObjectStorage();
    }

    /**
     * This method returns the shape factory.
     *
     * @return ShapeFactory the shape factory
     */
    public function getShapeFactory()
    {
        return $this->shapeFactory;
    }

    /**
     * This method returns the edge factory.
     *
     * @return EdgeFactory the edge factory
     */
    public function getEdgeFactory()
    {
        return $this->edgeFactory;
    }

    /**
     * This method sets the shape factory.
     *
     * @param ShapeFactory the new shape factory
     */
    public function setShapeFactory($shapeFactory)
    {
        $this->shapeFactory = $shapeFactory;
    }

    /**
     * This method sets the edge factory.
     *
     * @param EdgeFactory the new edge factory
     */
    public function setEdgeFactory($edgeFactory)
    {
        $this->edgeFactory = $edgeFactory;
    }

    /**
     * This method plots the tree with the given TreeStyle onto the given IDrawingPane
     *
     * @param TreeStyle $style the style to use for the plotting
     * @param DrawingPane $drawingPane the drawing pane to plot on
     * @return DrawingPane the DrawingPane, with the plotting of the tree
     */
    public function plot(TreeStyle $style, DrawingPane $drawingPane)
    {
        $this->treeStyle    = $style;

        $this->document     = $drawingPane;

        if(true)
            $this->plotIterativ($this->model->getRoot());

        else
            $this->plotRecursive($this->model->getRoot());

        return $this->document;
    }

    /**
     * This method plots the tree in an interative way.
     *
     * @param object $root the root node of the tree
     */
    private function plotIterativ($root)
    {
        $this->drawShape($root);

        $todo = array($root);

        while(count($todo) > 0)
        {
            $currentNode    = array_pop($todo);

            $childrenCount  = $this->model->getChildCount($currentNode);

            for($i = $childrenCount - 1; $i >= 0; --$i)
            {
                $target = $this->model->getChild($currentNode, $i);

                $todo[] = $target;

                $this->drawShape($target);
            }

            if($childrenCount > 0)
                $this->drawEdges($currentNode);
        }
    }

    /**
     * This method plots the tree in a recursive way.
     *
     * This might not be a good idea for real huge trees.
     *
     * @param object $root the current node to plot
     */
    private function plotRecursive($root)
    {
        $this->drawShape($root);

        $childrenCount  = $this->model->getChildCount($root);

        for($i = 0; $i < $childrenCount; $i++)
        {
            $target = $this->model->getChild($root, $i);

            $this->drawShape($target);

            $this->plotRecursive($target);
        }

        if($childrenCount > 0)
            $this->drawEdges($root);
    }

    /**
     * This method exports a node as a source shape.
     *
     * @param object $node the node to export
     * @return Shape the shape associated with the given source node
     */
    protected function drawShape($node)
    {
        $shape = $this->shapeFactory->createShape($this->treeStyle->shapeStyle,
                            $pos = $this->layout->getPosition($node, $this->treeStyle));
        $shape->draw($this->document);

        $text = $this->textFactory->createText($this->treeStyle->textStyle, $pos, $this->model->getLabel($node));
        $text->draw($this->document);

        $this->shapeMap[$node] = $shape;

        return $shape;
    }

    /**
     * This method exports the edges between the source and its target nodes.
     *
     * @param object $sourceNode the soure node
     */
    protected function drawEdges($sourceNode)
    {
        $edge = $this->edgeFactory->createEdge($this->treeStyle->edgeStyle,
                    $this->shapeMap[$sourceNode],
                    $this->getTargetShapes($sourceNode),
                    $this->treeStyle->shapeStyle);

        $edge->draw($this->document);
    }

    /**
     * This method returns the target shapes for a given source node.
     *
     * @param object $sourceNode the source node for which to get the target shapes
     * @return \ArrayObject the target shapes associated with the given source node
     */
    private function getTargetShapes($sourceNode)
    {
        $childrenCount  = $this->model->getChildCount($sourceNode);

        $targets        = new \ArrayObject();

        for($i = 0; $i < $childrenCount; $i++)
            $targets[$i] = $this->shapeMap[$this->model->getChild($sourceNode, $i)];

        return $targets;
    }
}