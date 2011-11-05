<?php

namespace Utils\Graphics2D\Shapes\Styles;

use Utils\Color\Color;

/**
 * This class implements a style for borders.
 */
class BorderStyle extends LineStyle
{
    /**
     * This acts as the constructor of the class.
     *
     * @param int $width the width of the border in pixels
     * @param Color $color the color of the border, or null for transparent color
     */
    public function __construct($width = 1, Color $color = null)
    {
        parent::__construct($width, null);
    }
}