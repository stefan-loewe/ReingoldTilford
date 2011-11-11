<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Color\Color;
use Utils\Color\RgbColor;

use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class implements a drawing pane for the HTML canvas element.
 */
class HtmlCanvasDrawingPane extends DrawingPane
{
    /**
     * This method acts as the constructor of the class.
     *
     * @param Dimension $dimension the dimension of the document
     */
    public function __construct(Dimension $dimension)
    {
        parent::__construct($dimension);

        $this->document = '';

        $this->document .= "\n".'var canvas = document.getElementById("rt_canvas");';
        $this->document .= "\n".'if(canvas.getContext)';
        $this->document .= "\n".'{';
        $this->document .= "\n"."\t".'var ctx = canvas.getContext("2d");';
    }

    /**
     * @inheritDoc This method tries to imitate SVG line drawing, which start at the source and ends before the target. The line is always drawn from top to bottom and from left to right, not matter if source and target respect this.
     */
    public function drawLine(Point $source, Point $target)
    {
        $this->document .= "\n".'ctx.beginPath();';
        $this->document .= "\n".'ctx.moveTo('.$source->x.', '.$source->y.');';
        $this->document .= "\n".'ctx.lineTo('.$target->x.', '.$target->y.');';
        $this->document .= "\n".'ctx.closePath();';
        $this->document .= "\n".'ctx.stroke();';

        return $this;
    }

    /**
     * @inheritDoc Using imageellipse of gdlib does not work here, as drawings with even width/height results in ellipses being one pixel too wide/high (this is a knownn bug in PHP GD Library)
     */
    public function drawEllipse(Point $center, Dimension $dimension)
    {
        return $this;
    }

    /**
     *
     */
    public function drawRectangle(Point $topLeftCorner, Dimension $dimension)
    {
        $this->document .= "\n".'ctx.strokeRect('.$topLeftCorner->x.', '.$topLeftCorner->y.', '.$dimension->width.', '.$dimension->height.');';

        return $this;
    }

    /**
     *
     */
    function drawPolygon(\Traversable $points)
    {
        return $this;
    }

    /**
     *
     */
    public function drawText(Point $topLeftCorner, $text)
    {
        return $this;
    }

    /**
     *
     */
    public function save()
    {
        $this->document .= "\n".'}';

        return $this->document;
    }
}
