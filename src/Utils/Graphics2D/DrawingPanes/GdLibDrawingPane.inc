<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Color\Color;
use Utils\Color\RgbColor;

use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class implements a drawing pane for the GD PHP extension.
 */
class GdLibDrawingPane extends DrawingPane
{
    /**
     * the file format of the document
     *
     * @var string
     */
    private $format             = null;

    /**
     * a flag determines whether to use true-color or not
     *
     * @var boolean
     */
    private $useTrueColor   = FALSE;

    /**
     * a collection of GD color ressources - this is a work-around for the limit of colors to be created in a non-true-color GD image
     *
     * @var int[string]
     */
    private $gdColors   = array();

    /**
     * the current path to search for fonts
     *
     * @var string
     */
    private $fontPath   = null;

    /**
     * the default path to search for fonts on Windows systems
     *
     * @var string
     */
    private static $DEFAULT_FONT_PATH_WIN   = 'C:/Windows/Fonts/';

    /**
     * the default path to search for fonts on *IX systems
     *
     * @var string
     */
    private static $DEFAULT_FONT_PATH_IX    = '/usr/share/fonts/';

    /**
     * This method acts as the constructor of the class.
     *
     * @param Dimension $dimension the dimension of the document
     * @param string $format the file format of the document, which is one of ImageDrawingPane::GIF, ImageDrawingPane::JPG, ImageDrawingPane::PNG
     * @param boolean $useTrueColor determines if the image should be a true color image (which consumes more memory)
     */
    public function __construct(Dimension $dimension, $format, $useTrueColor = TRUE)
    {
        parent::__construct($dimension);

        $this->initializeFontPath();

        if($format != ImageDrawingPane::GIF
                && $format != ImageDrawingPane::JPG
                && $format != ImageDrawingPane::PNG)
            throw new \InvalidArgumentException('invalid file type given');

        $this->format       = $format;

        $this->useTrueColor = $useTrueColor;

        if($this->useTrueColor)
        {
            // using true color to be able to use more than 256 colors
            $this->document = imagecreatetruecolor($this->dimension->width, $this->dimension->height);
            imagefill($this->document, 0, 0, imagecolorallocate($this->document, 255, 255, 255));
        }
        else
        {
            $this->document = imagecreate($this->dimension->width, $this->dimension->height);
            imagecolorallocate($this->document, 255, 255, 255);
        }
    }

    /**
     * This method initializes the font path based on the current operating system.
     */
    private function initializeFontPath()
    {
        if($this->fontPath === null)
        {
            $this->fontPath = strpos(\PHP_OS, 'WIN') === FALSE
                                ? self::$DEFAULT_FONT_PATH_IX
                                : self::$DEFAULT_FONT_PATH_WIN;
        }
    }

    /**
     * This method sets the font path to the given path.
     *
     * @param string $fontPath the new font path to use
     */
    public function setFontPath($fontPath)
    {
        $this->fontPath = str_replace('\\', '/', $fontPath);

        if(substr($this->fontPath, 0, -1) !== '/')
            $this->fontPath = $this->fontPath.'/';
    }

    /**
     * This method creates the internal GD library document.
     *
     * @return void
     */
    private function createImage()
    {
        if($this->format === ImageDrawingPane::GIF)
            imagegif($this->document);

        else if($this->format === ImageDrawingPane::JPG)
            imagejpeg($this->document);

        else if($this->format === ImageDrawingPane::PNG)
            imagepng($this->document);
    }

    /**
     * @inheritDoc This method tries to imitate SVG line drawing, which start at the source and ends before the target. The line is always drawn from top to bottom and from left to right, not matter if source and target respect this.
     */
    public function drawLine(Point $source, Point $target)
    {
        imagesetthickness($this->document, $this->strokeWidth);

        // draw horizontal lines from left to right
        if($source->y === $target->y)
            imageline($this->document, min($source->x, $target->x), $source->y, max($source->x, $target->x) - 1, $target->y, $this->toGDColor($this->strokeColor));

        // draw vertical lines from top to bottom
        else if($source->x === $target->x)
            imageline($this->document, $source->x, min($source->y, $target->y), $target->x, max($source->y, $target->y) - 1, $this->toGDColor($this->strokeColor));

        else if($source->y > $target->y)
        {
            if($source->x > $target->x)
                imageline($this->document, $target->x, $target->y, $source->x - 1, $source->y - 1, $this->toGDColor($this->strokeColor));
            else
                imageline($this->document, $target->x - 1, $target->y, $source->x, $source->y - 1, $this->toGDColor($this->strokeColor));
        }

        else if($source->x > $target->x)
            imageline($this->document, $target->x, $target->y - 1, $source->x - 1, $source->y, $this->toGDColor($this->strokeColor));

        else
            imageline($this->document, $source->x, $source->y, $target->x - 1, $target->y - 1, $this->toGDColor($this->strokeColor));

        return $this;
    }

    /**
     * @inheritDoc Using imageellipse of gdlib does not work here, as drawings with even width/height results in ellipses being one pixel too wide/high (this is a knownn bug in PHP GD Library)
     */
    public function drawEllipse(Point $center, Dimension $dimension)
    {
        // both stroke and fill are set, draw two filled arcs (for nicer results) ...
        if($this->strokeWidth > 0 && $this->fillColor)
        {
            // ... one for the "border" ...
            $this->drawFilledArc($center, $dimension->resizeBy(new Point($this->strokeWidth * 2, $this->strokeWidth * 2)), $this->strokeColor);

            // and one for the "filling"
            $this->drawFilledArc($center, $dimension);
        }

        // if only stroke is set, draw a arc only
        else if($this->strokeWidth != null)
            $this->drawArc($center, $dimension);

        // if only fill is set, only draw a filled arc
        else if($this->fillColor != null)
            $this->drawFilledArc($center, $dimension);

        return $this;
    }

    /**
     * This method draws a filled arc
     *
     * @param Point $center the center of the arc
     * @param Dimension $dimension the dimension of the arc
     * @param Color $fillColor the fill color of the arc, or null if not filled
     * @return GdLibDrawingPane $this
     */
    private function drawFilledArc(Point $center, Dimension $dimension, Color $fillColor = null)
    {
        imagesetthickness($this->document, 0);

        // imagearc does not support imagesetthickness in case a full arc is drawn, so draw two halfs
        imagefilledarc($this->document,
                $center->x,
                $center->y,
                $dimension->width,
                $dimension->height,
                0,
                180,
                $this->toGDColor(($fillColor == null) ? $this->fillColor : $fillColor),
                IMG_ARC_PIE);
        imagefilledarc($this->document,
                $center->x,
                $center->y,
                $dimension->width,
                $dimension->height,
                180,
                360,
                $this->toGDColor(($fillColor == null) ? $this->fillColor : $fillColor),
                IMG_ARC_PIE);

        return $this;
    }

    /**
     * This method draws a arc
     *
     * @param Point $center the center of the arc
     * @param Dimension $dimension the dimension of the arc
     * @return GdLibDrawingPane $this
     */
    private function drawArc(Point $center, Dimension $dimension)
    {
        $strokeWidth = $this->strokeWidth;

        imagesetthickness($this->document, 1);

        // imagearc leads to strange results with thicker strokes, so draw several strokes one-pixel-wide strokes
        while($strokeWidth > 0)
        {
            // imageellipse does not work, as imagesetthickness is not supported
            // imagearc does not support imagesetthickness in case a full arc is drawn, so draw two halfs
            imagearc($this->document,
                    $center->x,
                    $center->y,
                    $dimension->width + $strokeWidth * 2,
                    $dimension->height + $strokeWidth * 2,
                    0,
                    180,
                    $this->toGDColor($this->strokeColor));
            imagearc($this->document,
                    $center->x,
                    $center->y,
                    $dimension->width + $strokeWidth * 2,
                    $dimension->height + $strokeWidth * 2,
                    180,
                    360,
                    $this->toGDColor($this->strokeColor));

            $strokeWidth--;
        }
    }

    /**
     *
     */
    public function drawRectangle(Point $topLeftCorner, Dimension $dimension)
    {
        // with GDLib, a rectangle is always one pixel too wide and high, so we subtract 1 in each dimension
        $dimension = $dimension->resizeBy(new Point(-1, -1));

        if($this->strokeWidth > 0 && $this->strokeColor != null)
        {
            imagesetthickness($this->document, $this->strokeWidth);

            $borderTlc  = $topLeftCorner->moveBy(new Point(-$this->strokeWidth / 2, -$this->strokeWidth / 2));

            $borderDim  = $dimension->resizeBy(new Point($this->strokeWidth + $this->strokeWidth % 2, $this->strokeWidth + $this->strokeWidth % 2));

            imagerectangle($this->document,
                    $borderTlc->x,
                    $borderTlc->y,
                    $borderTlc->x + $borderDim->width,
                    $borderTlc->y + $borderDim->height,
                    $this->toGDColor($this->strokeColor));
        }

        if($this->fillColor != null)
        {
            imagesetthickness($this->document, 0);

            imagefilledrectangle($this->document,
                    $topLeftCorner->x,
                    $topLeftCorner->y,
                    $topLeftCorner->x + $dimension->width,
                    $topLeftCorner->y + $dimension->height,
                    $this->toGDColor($this->fillColor));
        }

        return $this;
    }

    /**
     *
     */
    function drawPolygon(\Traversable $points)
    {
        $vertices = array();
        foreach($points as $point)
        {
            $vertices[] = $point->x;
            $vertices[] = $point->y;
        }

        if($this->strokeWidth > 0 && $this->strokeColor != null)
        {
            imagesetthickness($this->document, $this->strokeWidth);

            imagepolygon($this->document,
                    $vertices,
                    count($vertices) / 2,
                    $this->toGDColor($this->strokeColor));
        }

        if($this->fillColor != null)
        {
            imagesetthickness($this->document, 0);

            imagefilledpolygon($this->document,
                $vertices,
                count($vertices) / 2,
                $this->toGDColor($this->fillColor));
        }

        return $this;
    }

    /**
     *
     */
    public function drawText(Point $topLeftCorner, $text)
    {
        $font = $this->fontPath.$this->fontFamily.'.ttf';

        imagettftext($this->document,
                        floor($this->fontSize * 3/4),
                        0,
                        $topLeftCorner->x,
                        ceil($topLeftCorner->y + $this->fontSize),
                        $this->toGDColor($this->fontColor),
                        $font,
                        $text);

        return $this;
    }

    /**
     *
     */
    public function save()
    {
        // start output buffering, ...
        ob_start();

        // ... create the image and print it to the buffer, ...
        $this->createImage();

        imagedestroy($this->document);

        // ... and finally get the buffer contents
        $imageStream = ob_get_contents();

        ob_end_clean();

        return $imageStream;
    }

    /**
     * This method converts a RGBColor to a GD color identifier.
     *
     * @param RGBColor $color the color to convert
     * @return int the GD color identifier for the given color
     */
    public function toGDColor(RGBColor $color)
    {
        return ($this->useTrueColor ? $this->toGDColorTC($color) : $this->toGDColorNTC($color));
    }

    /**
     * This method converts a RGBColor to a GD non-true-color identifier.
     *
     * @param RGBColor $color the color to convert
     * @return int the GD non-true-color identifier for the given color
     */
    private function toGDColorNTC($color)
    {
        if(!isset($this->gdColors[(string)$color->red.'|'.(string)$color->green.'|'.(string)$color->blue]))
            $this->gdColors[(string)$color->red.'|'.(string)$color->green.'|'.(string)$color->blue] = imagecolorallocate($this->document, $color->red, $color->green, $color->blue);

        return $this->gdColors[(string)$color->red.'|'.(string)$color->green.'|'.(string)$color->blue];
    }

    /**
     * This method converts a RGBColor to a GD true-color identifier.
     *
     * @param RGBColor $color the color to convert
     * @return int the GD true-color identifier for the given color
     */
    private function toGDColorTC($color)
    {
        return imagecolorallocate($this->document, $color->red, $color->green, $color->blue);
    }
}