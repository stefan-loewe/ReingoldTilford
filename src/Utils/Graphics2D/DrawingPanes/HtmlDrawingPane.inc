<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Color\Color;
use Utils\Color\RgbColor;

use Utils\Geom\Dimension;
use Utils\Geom\Point;

/**
 * This class implements a drawing pane for HTML output.
 */
class HtmlDrawingPane extends DomDrawingPane
{
    /**
     * the collection of used CSS styles
     *
     * @var stdClass[string]
     */
    protected $styleClasses         = array();

    /**
     * the flag to determine, whether or not CSS style information should be consolidated for output or not
     *
     * @var boolean
     */
    protected $consolidateStyles    = TRUE;

    /**
     * This method acts as the constructor of the class.
     *
     * @param Dimension $dimension the dimension of the document
     */
    public function __construct(Dimension $dimension)
    {
        parent::__construct($dimension);
    }

    /**
     * @inheritDoc In plain HTML, only horizonal or vertical lines are supported, otherwise a \InvalidArgumentException is thrown.
     */
    public function drawLine(Point $source, Point $target)
    {
        $style = null;

        // vertical line
        if($source->x == $target->x)
        {
            $style  = 'left:'.($source->x - $this->strokeWidth / 2).'px;'.
                            ' top:'.min($source->y, $target->y).'px;'.
                            ' width:'.$this->strokeWidth.'px;'.
                            ' height:'.(max($source->y, $target->y) - min($source->y, $target->y)).'px;';
        }

        // horizontal line
        else if($source->y == $target->y)
        {
            $style  = 'left:'.(min($source->x, $target->x)).'px;'.
                            ' top:'.($source->y - $this->strokeWidth / 2).'px;'.
                            ' width:'.(max($source->x, $target->x) - min($source->x, $target->x)).'px;'.
                            ' height:'.$this->strokeWidth.'px;';
        }

        else
            throw new \InvalidArgumentException('invalid source and target given, only horizontal or vertical lines can be drawn with plain html');

        $fixedStyle = 'border:none;'.
                        ' background-color:'.$this->toDOMColor($this->strokeColor).';';

        if($this->consolidateStyles)
            $class  = ' '.$this->addStyleClass($fixedStyle);
        else
        {
            $class  = '';

            $style  = $style.$fixedStyle;
        }

        $code = '<div'.
                    ' class="'.$this->classNameEdge.$class.'"'.
                    ' style="'.$style.'">'.
                '</div>';

        $this->fragment->appendXML($code);

        return $this;
    }

    /**
     * @inheritDoc Currently, this method is unsupported for HTMLDocument - a <code>BadMethodCallException</code> is thrown when it is called.
     */
    public function drawEllipse(Point $center, Dimension $dimension)
    {
        throw new \BadMethodCallException('arcs cannot be drawn with plain html');

        return $this;
    }

    public function drawRectangle(Point $topLeftCorner, Dimension $dimension)
    {
        $topLeftCorner = $topLeftCorner->moveBy(new Point(-$this->strokeWidth, -$this->strokeWidth));

        $style = 'left:'.($topLeftCorner->x).'px;top:'.($topLeftCorner->y).'px;';

        $constantStyle = 'width:'.($dimension->width).'px;'.
                        ' height:'.($dimension->height).'px;'.
                        ' border:solid '.$this->getBorderWidth().'px;'.
                        ' border-color:'.$this->getBorderColor().';'.
                        ' background-color:'.$this->getFillColor().';';

        // keep track of style definitions, so when exporting this, the same styles can be exported as a CSS class that gets referenced instead of reprinting it multiple times
        if($this->consolidateStyles)
            $class  = ' '.$this->addStyleClass($constantStyle);
        else
        {
            $class  = '';

            $style  = $style.$constantStyle;
        }

        $code = "\n".'<div class="'.$this->classNameNode.$class.'" style="'.$style.'"></div>';

        $this->fragment->appendXML($code);

        return $this;
    }

    /**
     * @inheritDoc Currently, this method is unsupported for HTMLDocument - a <code>BadMethodCallException</code> is thrown when it is called.
     */
    function drawPolygon(\Traversable $points)
    {
        throw new \BadMethodCallException('polygons cannot be drawn with plain html');

        return $this;
    }

    /**
     * @inheritDoc With font sizes bigger than 20 pixels, the HTML text is not correctly positioned, when compared the e.g. SVG or GDLib - reason not known, might be HTML rendering, but same for Firefox 4.0 and Internet Explorer 9
     */
    public function drawText(Point $topLeftCorner, $text)
    {
        $this->classNameText = 'text';

        $style = 'position: absolute;'.
                    ' top:'.ceil($topLeftCorner->y/* - ($this->fontSize * 3/4)*//* + ($this->fontSize / 20)*/).'px;'.
                    ' left:'.($topLeftCorner->x).'px;'.
                    ' font-family:'.$this->fontFamily.';'.
                    ' font-size:'.$this->fontSize.'px;'.
                    ' color:'.$this->toDOMColor($this->fontColor).';'.
                    ' white-space:nowrap;';

        $code =    "\n".'<div class="'.$this->classNameText.'" style="'.$style.'">'.nl2br(htmlentities($text)).'</div>';

        $this->fragment->appendXML($code);

        return $this;
    }

    /**
     *
     */
    public function save()
    {
        $this->fragment->appendXML($this->getCssCode());

        $this->appendFragment($this->document);

        $this->document->formatOutput = TRUE;

        return $this->document->saveHTML();
    }

    /**
     * This method returns the CSS code for the current document as string.
     *
     * @return type string the CSS code for the current document
     */
    private function getCssCode()
    {
        $cssCode = '<style>';

        foreach($this->styleClasses as $className => $style)
            $cssCode .= '.'.$style->name."\n".'{'.$style->data."\n".'}';

        return $cssCode.'</style>';
    }

    /**
     * This method adds a style definition to the set of known styles
     *
     * @param string $style the style information as string
     * @return string the generic name of the CSS class made from the given style definition
     */
    private function addStyleClass($style)
    {
        $hash = md5($style);

        if(!isset($this->styleClasses[$hash]))
        {
            $class       = new \stdClass();
            $class->name = 'sty'.count($this->styleClasses);
            $class->data = $style;

            $this->styleClasses[$hash]  = $class;
        }

        return $this->styleClasses[$hash]->name;
    }

    /**
     * This method is a short hand for getting the border width.
     *
     * @return int the border width
     */
    private function getBorderWidth()
    {
        return ($this->strokeWidth >= 0) ? $this->strokeWidth : 0;
    }

    /**
     * This method is a short hand for getting the border color as DOM color string.
     *
     * @return string the border color as DOM color string
     */
    private function getBorderColor()
    {
        return ($this->strokeColor != null) ? $this->toDOMColor($this->strokeColor) : 'none';
    }

    /**
     * This method is a short hand for getting the fill color as DOM color string.
     *
     * @return string the fill color as DOM color string
     */
    private function getFillColor()
    {
        return ($this->fillColor != null) ? $this->toDOMColor($this->fillColor) : 'none';
    }
}