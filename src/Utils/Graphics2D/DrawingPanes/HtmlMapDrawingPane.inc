<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Color\Color;
use Utils\Color\RgbColor;

use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class implements a drawing pane for HTML map output.
 */
class HtmlMapDrawingPane extends DomDrawingPane
{
    /**
     * the name of the HTML map element
     *
     * @var string
     */
    private $mapName = null;

    /**
     * This method acts as the constructor for the class.
     *
     * @param Dimension $dimension the dimension of the document
     */
    public function __construct(Dimension $dimension)
    {
        parent::__construct($dimension);

        $this->mapName = $mapName;
    }

    /**
     *
     */
    public function drawLine(Point $source, Point $target)
    {
        $points = new \ArrayObject();

        $points[] = $source->moveBy(new Point(-$this->strokeWidth * 2, 0));
        $points[] = $source->moveBy(new Point($this->strokeWidth * 2, 0));

        $points[] = $target->moveBy(new Point(-$this->strokeWidth * 2, 0));
        $points[] = $target->moveBy(new Point($this->strokeWidth * 2, 0));

        return $this->drawPolygon($points);
    }

    /**
     *
     */
    public function drawEllipse(Point $center, Dimension $dimension)
    {
        $code = '<area style="cursor: pointer;" shape="rect" coords="'.($center->x - $dimension->width / 2).
                                            ', '.($center->y - $dimension->height / 2).
                                            ', '.($center->x + $dimension->width / 2).
                                            ', '.($center->y + $dimension->height / 2).'"/>';

        $this->fragment->appendXML($code);

        return $this;
    }

    /**
     *
     */
    public function drawRectangle(Point $topLeftCorner, Dimension $dimension)
    {
        $topLeftCorner  = $topLeftCorner->moveBy(new Point(-$this->strokeWidth, -$this->strokeWidth));

        $dimension      = $dimension->resizeBy(new Point(2 * $this->strokeWidth, 2 * $this->strokeWidth));

        $code = '<area style="cursor: pointer;" shape="rect" coords="'.($topLeftCorner->x).
                ', '.($topLeftCorner->y).
                ', '.($topLeftCorner->x + $dimension->width).
                ', '.($topLeftCorner->y + $dimension->height).'"/>';

        $this->fragment->appendXML($code);

        return $this;
    }

    /**
     *
     */
    public function drawPolygon(\Traversable $points)
    {
        $pointsString = null;

        foreach($points as $point)
        {
            if($pointsString !== null)
                $pointsString = $pointsString.', ';

            $pointsString .= $point->x.', '.$point->y;
        }

        $code = '<area style="cursor:pointer;" shape="poly" coords="'.$pointsString.'"/>';

        $this->fragment->appendXML($code);

        return $this;
    }

    /**
     * @inheritDoc This method does not do anything, as drawing text on a HTML map does not make sense.
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
        $this->document->appendChild($mapElement = $this->document->createElement('map'));

        $mapElement->setAttributeNode(new \DOMAttr('name', $this->mapName));

        $this->appendFragment($mapElement);

        $this->document->formatOutput = TRUE;

        return $this->document->saveHTML();
    }
}