<?php

namespace Utils\Graphics2D\Shapes;

use Utils\Geom\Point;
use Utils\Graphics2D\Shapes\Styles\ShapeStyle;
use Utils\Graphics2D\Shapes\Styles\EdgeStyle;
use Utils\Graphics2D\DrawingPanes\DrawingPane;

/**
 * This class represents a direct edge.
 */
class DirectEdge extends Edge
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
        $document->setStrokeWidth($this->style->width)
                ->setStrokeColor($this->style->color);

        $source = $this->source->getConnectionPoint(Shape::SOUTH);

        // draw a direct line from the source to each target
        foreach($this->targets as $cnt => $target)
        {
            $document->drawLine($source,
                    $target->getConnectionPoint(Shape::NORTH));
        }
    }
}