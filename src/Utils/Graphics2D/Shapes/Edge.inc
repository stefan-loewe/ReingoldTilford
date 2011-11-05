<?php

namespace Utils\Graphics2D\Shapes;

use Utils\Geom\Point;
use Utils\Graphics2D\Shapes\Styles\ShapeStyle;
use Utils\Graphics2D\Shapes\Styles\EdgeStyle;

/**
 * This class represents an abstract edge shape.
 */
abstract class Edge extends Shape
{
    /**
     * the source of the edge
     *
     * @var IShape
     */
    protected $source       = null;

    /**
     * the collection of targets of the edge
     *
     * @var \ArrayAccess of IShapes
     */
    protected $targets      = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param IShape $source the source of the edge
     * @param \ArrayAccess $targets the collection of targets of the shape
     * @param EdgeStyle $edgeStyle the style of the edge
     */
    public function __construct(IShape $source, \ArrayAccess $targets, EdgeStyle $edgeStyle)
    {
        parent::__construct($edgeStyle);

        $this->source       = $source;

        $this->targets      = $targets;
    }

    /**
     *
     */
    public function getConnectionPoint($orientation, $targetIndex = null)
    {
        if($targetIndex == null)
            return $this->source->getConnectionPoint($orientation);

        else
            return $this->targets[$targetIndex]->getConnectionPoint($orientation);
    }
}