<?php

namespace Utils\Graphics2D\Shapes;

use Utils\Geom\Point;
use Utils\Graphics2D\Shapes\Styles\ShapeStyle;
use Utils\Graphics2D\Shapes\Styles\EdgeStyle;
use Utils\Graphics2D\DrawingPanes\DrawingPane;

/**
 * This class represents a poly-edge shape.
 */
class PolyEdge extends Edge
{
    /**
     * This acts as the constructor of the class.
     *
     * @param IShape $source the source of the edge
     * @param \ArrayAccess $targets the collection of targets of the shape
     * @param EdgeStyle $edgeStyle the style of the edge
     */
    public function __construct(IShape $source, \ArrayAccess $targets, EdgeStyle $edgeStyle)
    {
        parent::__construct($source, $targets, $edgeStyle);
    }

    /**
     *
     */
    public function draw(DrawingPane $document)
    {
        if($this->targets->count() > 0)
        {
            $document->setStrokeWidth($this->style->width)
                    ->setStrokeColor($this->style->color)
                    /*->setFillColor(null)*/;

            // get the connection points of the source, its first and last child
            $source = $this->source->getConnectionPoint(Shape::SOUTH);
            $childF = $this->targets[0]->getConnectionPoint(Shape::NORTH);
            $childL = $this->targets[$this->targets->count() - 1]->getConnectionPoint(Shape::NORTH);

            // the distance between the bottom edge of the source and the top edge of the targets
            $offset = ($childF->y - $source->y) / 2;

            // draw a horizontal connector from the first target to the last target, in the middle of the gap between the source and the targets
            if($this->targets->count() > 1)
                $document->drawLine($childF->moveBy(new Point(0, -$offset)),
                                        $childL->moveBy(new Point(0, -$offset)));

            // draw a vertical line from the bottom border of the source to the horizontal connector
            $document->drawLine($source, $source->moveBy(new Point(0, $offset)));

            // draw a vertical line from the top border of each target to the horizontal connector
            foreach($this->targets as $target)
            {
                $south  = $target->getConnectionPoint(Shape::NORTH);
                $north  = $south->moveBy(new Point(0, -$offset));
                $document->drawLine($north, $south);
            }
        }
    }
}