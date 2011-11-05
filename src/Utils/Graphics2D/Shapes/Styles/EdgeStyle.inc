<?php

namespace Utils\Graphics2D\Shapes\Styles;

use Utils\Color\Color;

/**
 * This class implements a style for edges.
 */
class EdgeStyle extends LineStyle
{
    /**
     * the identifier for a direct edge, i.e. a direct line from source to target
     */
    const DIRECT_EDGE       = 0;

    /**
     * the identifier for a poly edge, i.e. a connection from source to target via horizonal or vertical lines only
     */
    const POLY_EDGE         = 1;

    /**
     * the type of this edge, either self::DIRECT_EDGE or self::POLY_EDGE
     *
     * @var int
     */
    protected $type         = self::POLY_EDGE;

    /**
     * This acts as the constructor of the class.
     *
     * @param int $width the width of the edge
     * @param Color $color the color of the edge, or null for transparent color
     * @param int $type the type of edge, either self::DIRECT_EDGE for a direct line, or self::POLY_EDGE for a poly edge
     */
    public function __construct($width = 1, Color $color = null, $type = self::POLY_EDGE)
    {
        parent::__construct($width, $color);

        $this->type = $type;
    }
}