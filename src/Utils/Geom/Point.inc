<?php

namespace Utils\Geom;

/**
 * This class represents a point in the two-dimensional space.
 */
class Point
{
    /**
     * the x-coordinate of the point
     *
     * @var int
     */
    private $x = null;

    /**
     * the y-coordinate of the point
     *
     * @var int
     */
    private $y = null;

    /**
     * This method acts as the constructor of the class.
     *
     * @param int $x the x-coordinate of the point
     * @param int $y the y-coordinate of the point
     */
    public function __construct($x, $y)
    {
        $this->x = $x;

        $this->y = $y;
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
     * This method sets the x-coordinate of the point.
     *
     * @param int $x the new x-coordinate of the point
     * @return Point this point
     */
    public function setX($x)
    {
        $this->x = $x;

        return $this;
    }

    /**
     * This method sets the y-coordinate of the point.
     *
     * @param int $y the new y-coordinate of the point
     * @return Point this point
     */
    public function setY($y)
    {
        $this->y = $y;
    }

    /**
     * This method moves a point by an offset, given as another Point.
     *
     * @param Point $point the Point by which this Point shall be moved by
     * @return Point a new Point moved by the offset encapsulated by the given Point
     */
    public function moveBy(Point $point)
    {
        return new Point($this->x + $point->x, $this->y + $point->y);
    }

    /**
     * This method returns a string representation of the Point.
     *
     * @return string the string representation of the Point
     */
    public function __toString()
    {
        return 'x: '.$this->x.', y: '.$this->y;
    }
}