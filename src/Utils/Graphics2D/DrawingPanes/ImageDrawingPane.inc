<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Geom\Dimension;

/**
 * This class implements an abstract drawing pane for binary image output.
 */
abstract class ImageDrawingPane extends DrawingPane
{
    /**
     * the constant for output as GIF
     */
    const GIF = 'gif';

    /**
     * the constant for output as JPG
     */
    const JPG = 'jpg';

    /**
     * the constant for output as PNG
     */
    const PNG = 'png';

    /**
     * This method acts as the constructor for the class.
     *
     * @param Dimension $dimension the dimension of the document
     */
    public function __construct(Dimension $dimension)
    {
        parent::__construct($dimension);
    }
}