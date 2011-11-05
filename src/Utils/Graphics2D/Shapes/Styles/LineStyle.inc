<?php

namespace Utils\Graphics2D\Shapes\Styles;

use Utils\Color\Color;
use Utils\Color\RgbColor;

/**
 * This class implements an abstract style for lines.
 */
abstract class LineStyle implements IStyle
{
    /**
     * the width of the line
     *
     * @var int
     */
    protected $width = null;

    /**
     * the color of the line
     *
     * @var RGBColor
     */
    protected $color = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param int $width the width of the line
     * @param Color $color the color of the line
     */
    public function __construct($width = 1, Color $color = null)
    {
        $this->width = $width;

        $this->color = $color === null ? new RGBColor(0, 0, 0) : $color;
    }

    /**
     * This method is a magic getter method for the class.
     *
     * @todo replace this with Trait in PHP 5.4
     * @param string $memberName the name of the member to get
     * @return mixed the value of the member
     */
    public function __get($memberName)
    {
        return $this->$memberName;
    }

    /**
     * This method is a setter for the width of the line style.
     *
     * @param int $width the new width of the line style
     */
    public function setWidth($width)
    {
        $this->width = $width;
    }

    /**
     * This method is a setter for the color of the line style.
     *
     * @param Color $color the new color of the line style
     */
    public function setColor(Color $color)
    {
        $this->color = $color;
    }
}