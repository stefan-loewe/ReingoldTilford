<?php

namespace Utils\Graphics2D\Shapes\Styles;

use Utils\Color\Color;
use Utils\Color\RgbColor;

/**
 * This class implements a style for shapes.
 */
class ShapeStyle implements IStyle
{
    /**
     * the shape identifier for a rectangle
     */
    const RECTANGLE     = 0;

    /**
     * the shape identifier for an ellipse
     */
    const ELLIPSE       = 1;

    /**
     * the shape identifier for a star
     */
    const STAR          = 2;

    /**
     * the type of shape
     *
     * @var int
     */
    protected $type     = null;

    /**
     * the width of the shape
     *
     * @var int
     */
    protected $width    = null;

    /**
     * the height of the shape
     *
     * @var int
     */
    protected $height   = null;

    /**
     * the color of the shape
     *
     * @var Color
     */
    protected $color    = null;

    /**
     * the border of the shape
     *
     * @var BorderStyle
     */
    protected $border   = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param int $type the type of the shape, which has to be one of self::RECTANGLE, self::ELLIPSE or self::STAR
     * @param int $width the width of the shape
     * @param int $height the height of the shape
     * @param Color $color the color of the shape
     * @param BorderStyle $border the border of the shape
     */
    public function __construct($type = self::RECTANGLE, $width = 22, $height = 10, Color $color = null, BorderStyle $border = null)
    {
        $this->type     = $type;

        $this->width    = $width;

        $this->height   = $height;

        $this->color    = $color === null ? new RgbColor(255, 255, 255) : $color;

        $this->border   = $border === null ? new BorderStyle() : $border;
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
     * This method is a setter for the width of the shape style.
     *
     * @param int $width the new width of the shape
     */
    public function setWidth($width)
    {
        $this->width = $width;
    }

    /**
     * This method is a setter for the height of the shape style.
     *
     * @param int $height the new height of the shape
     */
    public function setHeight($height)
    {
        $this->height = $height;
    }

    /**
     * This method is a setter for the color of the shape style.
     *
     * @param Color $color the new color of the shape
     */
    public function setColor(Color $color)
    {
        $this->color = $color;
    }
}