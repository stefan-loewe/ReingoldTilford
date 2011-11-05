<?php

namespace Utils\Geom;

/**
 * This class encapsulates the width and height of an object in the two-dimensional space, e.g. a rectangle or an ellipse.
 */
class Dimension
{
    /**
     * the width of the dimension
     *
     * @var int
     */
    private $width  = null;

    /**
     * the height of the dimension
     *
     * @var int
     */
    private $height = null;

    /**
     * This method acts as the constructor of the class.
     *
     * @param int $width the width of the dimension
     * @param int $height the height of the dimension
     */
    public function __construct($width, $height)
    {
        $this->width    = $width;

        $this->height   = $height;
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
     * This method resizes a Dimension by the offset encoded by a Point.
     *
     * @param Point $point the offset by which the Dimension has to be resized
     * @return Dimension a new Dimension resized by the offset encoded in the given Point
    */
    public function resizeBy(Point $point)
    {
        return new Dimension($this->width + $point->x, $this->height + $point->y);
    }

    /**
     * This method returns a string representation of the Dimension.
     *
     * @return string the string representation of the Dimension
     */
    public function __toString()
    {
        return 'width: '.$this->width.' / height: '.$this->height;
    }
}
?>
