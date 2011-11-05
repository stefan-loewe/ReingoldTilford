<?php

namespace Utils\Graphics2D\Shapes;

use Utils\Geom\Point;
use Utils\Graphics2D\Shapes\Styles\EdgeStyle;

/**
 * This class acts as factory for graphical edge elements.
 */
final class EdgeFactory
{
    /**
     * This acts as the constructor of the class.
     */
    public function  __construct() {}

    /**
     * This method acts as a factory method for Edges.
     *
     * @param EdgeStyle $edgeStyle the style of the edge
     * @param IShape $source the source shape of the edge
     * @param \ArrayAccess $targets the collection of target shapes of the edge
     * @return Edge the new Edge
     */
    public function createEdge(EdgeStyle $edgeStyle, IShape $source, \ArrayAccess $targets)
    {
        if($edgeStyle->type === EdgeStyle::DIRECT_EDGE)
            return new DirectEdge($source, $targets, $edgeStyle);

        else if($edgeStyle->type === EdgeStyle::POLY_EDGE)
            return new PolyEdge($source, $targets, $edgeStyle);
    }
}