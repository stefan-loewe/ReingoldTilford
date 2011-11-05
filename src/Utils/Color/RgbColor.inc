<?php

namespace Utils\Color;

/**
 * This class encapsulates an RGB color.
 */
class RgbColor extends Color
{
    /**
     * the red color component
     *
     * @var int
     */
    protected $red        = 0;

    /**
     * the green color component
     *
     * @var int
     */
    protected $green    = 0;

    /**
     * the blue color component
     *
     * @var int
     */
    protected $blue        = 0;

    /**
     * This method is the constructor of the class.
     *
     * @param int $red the red color component
     * @param int $green the green color component
     * @param int $blue the blue color component
     * @return void
     */
    public function __construct($red, $green, $blue)
    {
        parent::__construct();

        $this->red      = $red % 256;

        $this->green    = $green % 256;

        $this->blue     = $blue % 256;
    }

    /**
     * This method returns the DOM representation of the color, e.g. #000000 for black.
     *
     * @return string the color as string in its DOM representation
     */
    public function __toString()
    {
        return '#'.
            str_pad(dechex($this->red), 2, '0').
            str_pad(dechex($this->green), 2, '0').
            str_pad(dechex($this->blue), 2, '0');
    }
}