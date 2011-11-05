<?php

namespace Utils\Graphics2D\Shapes\Styles;

use Utils\Color\Color;
use Utils\Color\RgbColor;

/**
 * This class implements a style for texts.
 */
class TextStyle implements IStyle
{
    /**
     * constant additional padding for calculating the line height with a given font size (always plus 3)
     */
    const LINE_PADDING = 3;

    /**
     * the name of the font family
     *
     * @var string
     */
    private $family = 'Times';

    /**
     * the font size in pixel
     *
     * @var int
     */
    private $size   = '12';

    /**
     * the font color
     *
     * @var Color
     */
    private $color  = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param string $family the name of the font family
     * @param int $size the font size in pixel
     * @param Color $color the font color
     */
    public function __construct($family = 'Times', $size = 12, Color $color = null)
    {
        $this->family   = $family;

        $this->size     = $size;

        $this->color    = $color === null ? new RgbColor(0, 0, 0) : $color;
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
     * This method is a magic setter method for the class.
     *
     * @todo replace this with Trait in PHP 5.4
     * @param string $memberName the name of the member to get
     * @param mixed the new value of the member
     */
    public function __set($memberName, $value)
    {
        $this->$memberName = $value;
    }

    /**
     * This method returns the line height for this TextStyle.
     *
     * @return int the line height for this TextStyle in pixels
     */
    public function getLineHeight()
    {
        return $this->size + self::LINE_PADDING;
    }

    /**
     * This method converts from pixel to points unit.
     *
     * @param int $pixel the value in pixels
     * @return float the value in points
     */
    public static function pixelToPoint($pixel)
    {
        return $pixel * 3/4;
    }

    /**
     * This method returns a string representation of the text style.
     *
     * @return string
     */
    public function __toString()
    {
        return $this->family.' - '.$this->size.'px - '.$this->color;
    }

    /**
     * This method converts this TextStyle into a CSS class definition, with the given class name.
     *
     * @param string $className the class name of the CSS class definition
     * @return string a CSS class definition with the given class name of this TextStlye
     */
    public function toCssClass($className)
    {
        return '.'.$className
            .'{font-family:'.$this->family.';'
            .'font-size:'.$this->size.'px;'
            .'color:'.$this->color.';}';
    }

    /**
     * This method returns a "hash" of this TextStyle.
     *
     * @return string a "hash" of this TextStyle, consisting of the concatination of the fontFamily and size members of this TextStyle
     */
    public function getHash()
    {
        return $this->family.$this->size;
    }
}