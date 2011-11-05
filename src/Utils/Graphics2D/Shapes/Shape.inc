<?php

namespace Utils\Graphics2D\Shapes;

use Utils\Graphics2D\Shapes\Styles\IStyle;
use Utils\Graphics2D\DrawingPanes\DrawingPane;

/**
 * This class represents an abstract shape.
 */
abstract class Shape implements IShape
{
    /**
     * the identifier for a connection point facing north
     */
    const NORTH = 0;

    /**
     * the identifier for a connection point facing south
     */
    const SOUTH = 1;

    /**
     * the style of the shape
     *
     * @var IStyle
     */
    protected $style = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param IStyle $style the style of the shape
     */
    protected function __construct(IStyle $style)
    {
        $this->style = $style;
    }

    /**
     * This method draws the shape onto the IDrawingPane.
     *
     * @param DrawingPane $drawingPane
     */
    abstract public function draw(DrawingPane $drawingPane);

    /**
     * This method returns the connection points of the Shape.
     *
     * @param int $orientation where the connections should face to, either Shape::NORTH or Shape::SOUTH
     */
    abstract public function getConnectionPoint($orientation);
}