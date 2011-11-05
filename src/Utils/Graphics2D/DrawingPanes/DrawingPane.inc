<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Color\Color;

use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class implements an abstract drawing pane.
 */
abstract class DrawingPane implements IDrawingPane
{
    /**
     * the wrapped document
     *
     * @var mixed
     */
    public $document        = null;

    /**
     * the dimension of the document
     *
     * @var Dimension
     */
    protected $dimension    = 0;

    /**
     * the currect stroke width of the drawing pane
     *
     * @var int
     */
    protected $strokeWidth  = 1;

    /**
     * the current stroke color of the drawing pane
     *
     * @var Color
     */
    protected $strokeColor  = null;

    /**
     * the current fill color of the drawing pane
     *
     * @var Color
     */
    protected $fillColor    = null;

    /**
     * the current font family of the drawing pane
     *
     * @var string
     */
    protected $fontFamily  = null;

    /**
     * the current font size in pixels of the drawing pane
     *
     * @var int
     */
    protected $fontSize    = null;

    /**
     * the current font color of the drawing pane
     *
     * @var Color
     */
    protected $fontColor    = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param Dimension $dimension the dimension of the document
     */
    public function __construct(Dimension $dimension)
    {
        $this->dimension = $dimension;
    }

    /**
     *
     */
    public function drawPolyLine(\Traversable $points)
    {
        $pointsArray = new \ArrayObject();
        foreach($points as $point)
            $pointsArray[] = $point;

        if($pointsArray->count() < 2)
            throw new \InvalidArgumentException('a polyline must have at least two points');

        for($i = 0, $count = $points->count(); $i < $count - 1; $i++)
            $this->drawLine($points[$i], $points[$i + 1]);

        return $this;
    }

    /**
     *
     */
    public function setStrokeWidth($strokeWidth)
    {
        $this->strokeWidth = $strokeWidth;

        return $this;
    }

    /**
     *
     */
    public function setStrokeColor(Color $strokeColor)
    {
        $this->strokeColor = $strokeColor;

        return $this;
    }

    /**
     *
     */
    public function setFillColor(Color $fillColor)
    {
        $this->fillColor = $fillColor;

        return $this;
    }

    /**
     *
     */
    public function setFontFamily($fontFamily)
    {
        $this->fontFamily = $fontFamily;

        return $this;
    }

    /**
     *
     */
    public function setFontSize($fontSize)
    {
        $this->fontSize = $fontSize;

        return $this;
    }

    /**
     *
     */
    public function setFontColor(Color $fontColor)
    {
        $this->fontColor = $fontColor;

        return $this;
    }
}