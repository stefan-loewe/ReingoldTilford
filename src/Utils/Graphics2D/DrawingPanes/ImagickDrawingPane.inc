<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Color\Color;
use Utils\Color\RgbColor;

use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class implements a drawing pane for Imagick PHP extension.
 */
class ImagickDrawingPane extends ImageDrawingPane
{
    /**
     * the drawing pane of the Imagick object
     *
     * @var ImagickDraw
     */
    private $draw   = null;

    /**
     * the output file type of the visualisation, which is one of ImageDrawingPane::GIF, ImageDrawingPane::JPG, ImageDrawingPane::PNG
     *
     * @var string
     */
    private $format = ImageDrawingPane::PNG;

    /**
     * This method acts as the constructor for the class.
     *
     * @param Dimension $dimension the dimension of the document
     */
    public function __construct(Dimension $dimension)
    {
        parent::__construct($dimension);

        $this->document = new \Imagick();

        $this->document->newImage($this->dimension->width, $this->dimension->height, new \ImagickPixel('white'));

        $this->draw = new \ImagickDraw();

        $this->draw->setStrokeAntialias(false);
    }

    /**
     *
     */
    public function drawLine(Point $source, Point $target)
    {
        // draw horizontal lines from left to right
        if($source->y === $target->y)
            $this->draw->line(min($source->x, $target->x), $source->y, max($source->x, $target->x) - 1, $target->y);

        // draw vertical lines from top to bottom
        else if($source->x === $target->x)
            $this->draw->line($source->x, min($source->y, $target->y), $target->x, max($source->y, $target->y) - 1);

        // imitates SVG line drawing, which start at the point with the lower
        // y coordinate and it also subtracts one pixel in length
        else if($source->y > $target->y)
        {
            if($source->x > $target->x)
                $this->draw->line($target->x, $target->y, $source->x - 1, $source->y - 1);
            else
                $this->draw->line($target->x - 1, $target->y, $source->x, $source->y - 1);
        }

        else if($source->x > $target->x)
            $this->draw->line($target->x, $target->y - 1, $source->x - 1, $source->y);

        else
            $this->draw->line($source->x, $source->y, $target->x - 1, $target->y - 1);

        return $this;
    }

    /**
     *
     */
    public function drawPolyLine(\Traversable $points)
    {
        $this->setFillColor(new \ImagickPixel('transaprent'));
        $this->draw->polyline($this->pointsToArray($points));

        return $this;
    }

    /**
     *
     */
    public function drawEllipse(Point $center, Dimension $dimension)
    {
        $this->draw->ellipse($center->x,
                $center->y,
                $dimension->width / 2 + $this->strokeWidth / 2,
                $dimension->height / 2 + $this->strokeWidth / 2,
                0,
                360);

        return $this;
    }

    /**
     * @inheritDoc Currently there is the limitations, that the border width is on pixel to wide, unless it is 0 or 1
     */
    public function drawRectangle(Point $topLeftCorner, Dimension $dimension)
    {
        // for some reason, stroke-width is one pixel too wide, so subtract 1
        if(($correctStrokeWidth = ($this->strokeWidth == 1)))
            $this->setStrokeWidth($this->strokeWidth - 0.5);
        else
            $this->setStrokeWidth($this->strokeWidth - 1);

        // border "grows" in both directions, so subtract half the stroke width
        // subtract one more pixel in each dimension, as it's positioned wrong
        $topLeftCorner  = $topLeftCorner->moveBy(new Point(-$this->strokeWidth / 2 - 1, -$this->strokeWidth / 2 - 1));

        // for some reason rectangle is one pixel to small, so add one pixel in each dimension
        $dimension      = $dimension->resizeBy(new Point($this->strokeWidth + 1, $this->strokeWidth + 1));

        $this->draw->rectangle($topLeftCorner->x,
                $topLeftCorner->y,
                $topLeftCorner->x + $dimension->width,
                $topLeftCorner->y + $dimension->height);

        // reset stroke-width to original value
        if($correctStrokeWidth)
            $this->setStrokeWidth($this->strokeWidth + 0.5);
        else
            $this->setStrokeWidth($this->strokeWidth + 1);

        return $this;
    }

    /**
     *
     */
    function drawPolygon(\Traversable $points)
    {
        $this->draw->polygon($this->pointsToArray($points));

        return $this;
    }

    /**
     *
     */
    public function drawText(Point $topLeftCorner, $text)
    {
        $this->draw->setFont($this->fontFamily);
        $this->draw->setStrokeWidth(0);
        $this->draw->setFontSize($this->fontSize);
        $this->draw->setFillColor($this->toImagikColor($this->fontColor));

        $this->document->annotateImage($this->draw, $topLeftCorner->x, $topLeftCorner->y + $this->fontSize, 0, $text.'dasdsfsdfsdfsdfsdfsdfsdfsdfsdfsdfsdflfs');

        $this->draw->setStrokeWidth($this->strokeWidth);
        $this->draw->setFillColor($this->toImagikColor($this->fillColor));

        return $this;
    }

    /**
     * This method sets the format in which the visualisation will be exported.
     *
     * @param string the desired file format, which is one of ImageDrawingPane::GIF, ImageDrawingPane::JPG, ImageDrawingPane::PNG
     * @return ImagickDrawingPane $this
     */
    public function setFormat($format)
    {
        $this->format = $format;

        return $this;
    }

    /**
     *
     */
    public function save()
    {
        $this->document->drawImage($this->draw);

        $this->document->setImageFormat($this->format);

        // start buffering ...
        ob_start();

        // ... print the image to the buffer ...
         echo $this->document;

        // ... retrieve the data from the buffer ...
        $result = ob_get_contents();

        // ... clear the buffer ...
        ob_end_clean();

        // ... and return the image
        return $result;
    }

    /**
     * This method converts a color to its Imagik color representation.
     *
     * @param RGBColor $color the color to convert
     * @return string the Imagik color representation of the color
     */
    public function toImagikColor(RGBColor $color)
    {
        return '#'.
                str_pad(dechex($color->red), 2, '0').
                str_pad(dechex($color->green), 2, '0').
                str_pad(dechex($color->blue), 2, '0');
    }

    /**
     * This method converts a collection of Points into a two-dimenstional array of x- and y-coordinates.
     *
     * The resulting array looks like array[[x => 137, y = 64], [x => 129, y => 12]... , [x => 225, y => 358]]
     *
     * @param \Traversable $points the collection of Points to convert
     * @return array[] the two-dimenstional array of x- and y-coordinates
     */
    private function pointsToArray(\Traversable $points)
    {
        $pointsArray = array();

        foreach($points as $point)
            $pointsArray[] = array('x' => $point->x, 'y' => $point->y);

        return $pointsArray;
    }

    /**
     *
     */
    public function setStrokeWidth($strokeWidth)
    {
        $this->strokeWidth = $strokeWidth;

        $this->draw->setStrokeWidth($this->strokeWidth);

        if($this->strokeWidth == 0)
            $this->draw->setStrokeColor(new \ImagickPixel('transparent'));

        return $this;
    }

    /**
     *
     */
    public function setStrokeColor(Color $strokeColor)
    {
        $this->strokeColor = $strokeColor;

        $this->draw->setStrokeColor($this->toImagikColor($this->strokeColor));

        return $this;
    }

    /**
     *
     */
    public function setFillColor(Color $fillColor)
    {
        $this->fillColor = $fillColor;

        $this->draw->setFillColor($this->toImagikColor($this->fillColor));

        return $this;
    }
}
?>