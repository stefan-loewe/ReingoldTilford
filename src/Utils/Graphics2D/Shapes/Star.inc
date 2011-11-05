<?php

namespace Utils\Graphics2D\Shapes;

use Utils\Graphics2D\Shapes\Styles\ShapeStyle;
use Utils\Graphics2D\DrawingPanes\DrawingPane;
use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class represents a star shape.
 */
class Star extends Shape
{
    /**
     * the angle between two adjacent points of the pentagon, measured in the center of the pentagon
     *
     * @var double
     */
    private static $INNER_ANGLE                 = 72;

    /**
     * factor to get the side length of the pentagram from the diagonal of the pentagram
     *
     * @var double
     */
    private static $DIAGONAL_TO_LENGTH          = 0.6180339887/*4989484820458683436569*/;

    /**
     * factor to get the side length of the pentagram from the height of the pentagram
     *
     * @var double
     */
    private static $HEIGHT_TO_LENGTH            = 0.6498393924/*6581265231174282443046*/;

    /**
     * factor to get the radius of the circumcircle of the outter pentagram from the side length of the pentagram
     *
     * @var double
     */
    private static $LENGTH_TO_OUTTER_RADIUS     = 1.1755705045/*849462583374119092781*/;

    /**
     * factor to get the radius of the circumcircle of the inner pentagon from the diagonal of the outter pentagram
     *
     * @see http://mathworld.wolfram.com/Pentagram.html
     * @var double
     */
    private static $DIAGONAL_TO_INNER_RADIUS    = 0.2008114158/*8622727986979767263375*/;

    /**
     * the center of the Star
     *
     * @var Point
     */
    private $center = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param Point $center the center of the Star.
     * @param ShapeStyle $style the style of the Star.
     */
    public function __construct(Point $center, ShapeStyle $style)
    {
        parent::__construct($style);

        $this->center = $center;
    }

    /**
     *
     */
    public function draw(DrawingPane $document)
    {
        $document->setStrokeWidth($this->style->border->width)
                ->setStrokeColor($this->style->border->color)
                ->setFillColor($this->style->color)
                ->drawPolygon($this->getPoints($this->style->width + $this->style->border->width * 2, $this->style->height + $this->style->border->width * 2));
    }

    /**
     * This method gets the points to draw a 5-star.
     *
     * @param int $width the width of the Star
     * @param int $height the height of the Star
     * @return \ArrayObject of the Points of the Star
     */
    private function getPoints($width, $height)
    {
        // get the side length based on the given width value
        $lengthWidth    = $width * self::$DIAGONAL_TO_LENGTH;
        // get the side length based on the given height value
        $lengthHeight   = $height * self::$HEIGHT_TO_LENGTH;

        // get the radius of the circumcircle of the outter pentagon
        $outterRadius   = min($lengthWidth, $lengthHeight) / self::$LENGTH_TO_OUTTER_RADIUS;

        // get the radius of the circumcircle of the inner pentagon
        // FIXME: if $lengthHeight was picked as length, this is wrong!
        $innerRadius    = $width * self::$DIAGONAL_TO_INNER_RADIUS;

        $points = new \ArrayObject();

        // get the coordinates of the corners of the figure
        for($i = 0; $i < 360; $i = $i + self::$INNER_ANGLE)
        {
            // add a point of the outter pentagon ...
            $rad        = deg2rad($i);
            $point      = new Point(-sin($rad) * $outterRadius, -cos($rad) * $outterRadius);
            $points[]   = $point->moveBy($this->center);

            // ... followed by a point of the outter pentagon
            $rad        = deg2rad($i + self::$INNER_ANGLE / 2);
            $point      = new Point(-sin($rad) * $innerRadius, -cos($rad) * $innerRadius);
            $points[]   = $point->moveBy($this->center);
        }

        return $points;
    }

    /**
     *
     */
    public function getConnectionPoint($orientation)
    {
        return $this->center;
    }
}