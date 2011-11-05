<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Color\Color;
use Utils\Color\RgbColor;

use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class implements a drawing pane for SVG output.
 */
class SvgDrawingPane extends DomDrawingPane
{
    /**
     * This method acts as the constructor for the class.
     *
     * @param Dimension $dimension the dimension of the document
     */
    public function __construct(Dimension $dimension)
    {
        parent::__construct($dimension);
    }

    /**
     *
     */
    public function drawLine(Point $source, Point $target)
    {
        $this->drawPolyLine(new \ArrayObject(array($source, $target)));

        return $this;
    }

    /**
     *
     */
    public function drawPolyLine(\Traversable $points)
    {
        if($this->pointCount($points) < 2)
            throw new \InvalidArgumentException('a polyline must have at least two points');

        // if stroke width is odd, move every point of the line by a quater pixel in both dimension
        if($this->strokeWidth % 2)
        {
            foreach($points as $index => $point)
                $points[$index] = $points[$index]->moveBy(new Point(0.25, 0.25));
        }

        $this->fragment->appendXML('<polyline'
            .' points="'.$this->pointsToString($points).'"'
            .' style="fill:none;'
                .' stroke:'.$this->toDOMColor($this->strokeColor).';'
                .' stroke-width:'.$this->strokeWidth.';"'
            .'/>');

        return $this;
    }

    /**
     *
     */
    public function drawEllipse(Point $center, Dimension $dimension)
    {
        $this->fragment->appendXML('<ellipse'
            .' cx="'.$center->x.'"'
            .' cy="'.$center->y.'"'
            .' rx="'.(($dimension->width / 2) + ($this->strokeWidth / 2)).'"'
            .' ry="'.(($dimension->height / 2) + ($this->strokeWidth / 2)).'"'
            .$this->getStyleAttribute()
            .' />');

        return $this;
    }

    /**
     *
     */
    public function drawPolygon(\Traversable $points)
    {
        $this->fragment->appendXML('<polygon'
            .' class="node"'
            .' points="'.$this->pointsToString($points).'"'
            .$this->getStyleAttribute()
            .' />');

        return $this;
    }

    /**
     *
     */
    public function drawRectangle(Point $topLeftCorner, Dimension $dimension)
    {
        $topLeftCorner  = $topLeftCorner->moveBy(new Point(-$this->strokeWidth / 2, -$this->strokeWidth / 2));
        $dimension      = $dimension->resizeBy(new Point($this->strokeWidth, $this->strokeWidth));

        $this->fragment->appendXML('<rect'
            .' class="node"'
            .' x="'.($topLeftCorner->x).'"'
            .' y="'.($topLeftCorner->y).'"'
            .' width="'.($dimension->width).'"'
            .' height="'.($dimension->height).'"'
            .$this->getStyleAttribute()
            .' />');

        return $this;
    }

    /**
     *
     */
    public function drawText(Point $topLeftCorner, $text)
    {
        $x = $topLeftCorner->x;
        $y = ceil($topLeftCorner->y + $this->fontSize);

        $lines = null;
        foreach(explode(\PHP_EOL, $text) as $lineNumber => $line)
            $lines = $lines.'<tspan x="'.$x.'" y="'.($y + $lineNumber * ($this->fontSize + 3)).'">'.htmlentities($line).'</tspan>';

        $this->fragment->appendXML('<text x="'.$x.'"'
                                        .' y="'.$y.'"'
                                        .' style="font-family:'.$this->fontFamily.'; font-size:'.$this->fontSize.'px;"'
                                        .' fill="'.$this->toDOMColor($this->fontColor).'">'
                                            .$lines
                                    .' </text>');

        return $this;
    }

    /**
     *
     */
    public function save()
    {
        $this->clearDocument();

        // create and append a <svg> root element ...
        $this->document->appendChild($svgElement = $this->document->createElement('svg'));

        $svgElement->setAttributeNode(new \DOMAttr('xmlns', 'http://www.w3.org/2000/svg'));
        $svgElement->setAttribute('width', $this->dimension->width);
        $svgElement->setAttribute('height', $this->dimension->height);

        // .. as well as a <g> root element ...
        $svgElement->appendChild($rootElement = $this->document->createElement('g'));

        $rootElement->setAttribute('x', $this->dimension->width);
        $rootElement->setAttribute('y', $this->dimension->height);

        // deactivate anti-aliasing on all edges and nodes
        $rootElement->setAttribute('style', 'shape-rendering:crispedges;');

        // ... and append the generated fragment to it
        $this->appendFragment($rootElement);

        //$this->document->formatOutput = TRUE;

        return $this->document->saveXML();
    }

    /**
     * This method converts a collection of Points to a string representation suitable for SVG output of polylines.
     *
     * @param \Traversable $points the collection of Points
     * @return string the string representation of the collection of Points, i.e. "x1, y1 x2, y2 ... xn, yn"
     */
    private function pointsToString(\Traversable $points)
    {
        $pointsString = null;

        foreach($points as $point)
        {
            if($pointsString !== null)
                $pointsString = $pointsString.' ';

            $pointsString = $pointsString.$point->x.','.$point->y;
        }

        return $pointsString;
    }

    /**
     * This method determines the number of Points in the given collection.
     *
     * @param \Traversable $points the collection of Points
     * @return int the number of points in the collection
     */
    private function pointCount(\Traversable $points)
    {
        $count = 0;

        foreach($points as $point)
            ++$count;

        return $count;
    }

    /**
     * This method returns the constant SVG style attributes for the shapes.
     *
     * @return string
     */
    private function getStyleAttribute()
    {
        $style = ' style="';

        $strokeWidth = ($this->strokeWidth >= 0) ? $this->strokeWidth : 0;
        $style .= 'stroke-width:'.$strokeWidth.'px;';

        $strokeColor = ($this->strokeColor != null) ? $this->toDOMColor($this->strokeColor) : 'none';
        $style .= 'stroke:'.$strokeColor.';';

        $fillColor = ($this->fillColor != null) ? $this->toDOMColor($this->fillColor) : 'none';
        $style .= 'fill:'.$fillColor.';';

        return $style.'"';
    }
}