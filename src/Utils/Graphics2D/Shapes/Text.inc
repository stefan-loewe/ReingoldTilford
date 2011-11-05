<?php

namespace Utils\Graphics2D\Shapes;

use Utils\Graphics2D\Shapes\Styles\TextStyle;
use Utils\Graphics2D\DrawingPanes\DrawingPane;
use Utils\Geom\Point;

/**
 * This class represents a text shape.
 */
class Text extends Shape
{
    /**
     * the top-left corner of the Text
     *
     * @var Point
     */
    private $topLeftCorner  = null;

    /**
     * the actual text of the Text
     *
     * @var string
     */
    private $text           = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param Point $topLeftCorner the top-left corner where to position the text
     * @param TextStyle $textStyle the textStyle to use for the text.
     * @param string $treeNode the actual text of the Text
     */
    public function __construct(Point $topLeftCorner, TextStyle $style, $text)
    {
        parent::__construct($style);

        $this->topLeftCorner    = $topLeftCorner;

        $this->text             = $text;
    }

    /**
     *
     */
    public function draw(DrawingPane $drawingPane)
    {
        $drawingPane->setFontFamily($this->style->family)
                ->setFontSize($this->style->size)
                ->setFontColor($this->style->color)
                ->drawText($this->topLeftCorner, $this->text);
    }

    /**
     * @todo: tobe implemented later
     */
    public function getConnectionPoint($orientation)
    {
        return null;
    }
}