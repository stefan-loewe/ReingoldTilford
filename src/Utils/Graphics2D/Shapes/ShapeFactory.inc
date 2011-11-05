<?php

namespace Utils\Graphics2D\Shapes;

use Utils\Geom\Point;
use Utils\Graphics2D\Shapes\Styles\ShapeStyle;

/**
 * This class acts as factory for graphical shape elements.
 */
final class ShapeFactory
{
    /**
     * This acts as the constructor of the class.
     */
    public function  __construct() {}

    /**
     * This method acts as factory method for Shapes.
     *
     * @param ShapeStyle $shapeStyle the style of the shape
     * @param Point $topLeftCorner the position of the top-left corner of the shape
     * @return Shape the new Shape
     */
    public function createShape(ShapeStyle $shapeStyle, Point $topLeftCorner)
    {
        if($shapeStyle->type === ShapeStyle::RECTANGLE)
            return new Rectangle($topLeftCorner, $shapeStyle);

        else if($shapeStyle->type === ShapeStyle::ELLIPSE)
            return new Ellipse($topLeftCorner->moveBy(new Point($shapeStyle->width / 2, $shapeStyle->height / 2)), $shapeStyle);

        else if($shapeStyle->type === ShapeStyle::STAR)
            return new Star($topLeftCorner->moveBy(new Point($shapeStyle->width / 2, $shapeStyle->height / 2)), $shapeStyle);
    }
}