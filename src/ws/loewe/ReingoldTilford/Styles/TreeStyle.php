<?php

namespace ws\loewe\ReingoldTilford\Styles;

use \ws\loewe\Utils\Graphics2D\Shapes\Styles\IStyle;
use \ws\loewe\Utils\Graphics2D\Shapes\Styles\ShapeStyle;
use \ws\loewe\Utils\Graphics2D\Shapes\Styles\EdgeStyle;
use \ws\loewe\Utils\Graphics2D\Shapes\Styles\TextStyle;

/**
 * This class implements a style for trees.
 */
class TreeStyle implements IStyle
{
    /**
     * the identifier for a left-bound orientation of child nodes
     */
    const LEFT_BOUND        = 0;

    /**
     * the identifier for a centered orientation of child nodes
     */
    const CENTERED          = 1;

    /**
     * the identifier for a right-bound orientation of child nodes
     */
    const RIGHT_BOUND       = 2;

    /**
     * the style of the shapes
     *
     * @var ShapeStyle
     */
    private $shapeStyle     = null;

    /**
     * the style of the edges
     *
     * @var EdgeStyle
     */
    private $edgeStyle      = null;

    /**
     * the style of the text
     *
     * @var TextStyle
     */
    private $textStyle      = null;

    /**
     * the offset of the left of a node to the left to the node right next to it on the same level (meassured in the measuring unit given by TreeStyle::unit)
     *
     * @var int
     */
    private $xOffset        = null;

    /**
     * the offset from the top of a father to the top of its sons (meassured in the measuring unit given by TreeStyle::unit)
     * for a radial layout, this is used to determine the radius increment between levels
     *
     * @var int
     */
    private $yOffset        = null;

    /**
     * the offset from the top of the container to the top of the root (meassured in the measuring unit given by TreeStyle::unit)
     *
     * @var int
     */
    private $xPadding       = null;

    /**
     * the offset from the left of the container to the left of the most left node (meassured in the measuring unit given by TreeStyle::unit)
     *
     * @var int
     */
    private $yPadding       = null;

    /**
     * the measuring unit of the layout
     *
     * @var string
     */
    private $unit           = null;

    /**
     * the orientation of the tree, either one of left, center or right hierarchical
     *
     * @var int
     */
    private $orientation    = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param ShapeStyle $shapeStyle the style of the shapes
     * @param EdgeStyle $edgeStyle the style of the edges
     * @param TextStyle $textStyle  the style of the text
     * @param type $xOffset the x-offset
     * @param type $yOffset the y-offset
     * @param type $xPadding the x-padding
     * @param type $yPadding the y-padding
     * @param type $unit the unit to use
     * @param type $orientation the orientation of the child nodes, one of self::LEFT_BOUND, self::CENTERED, self::RIGHT_BOUND
     */
    public function __construct(ShapeStyle $shapeStyle = null,
                                EdgeStyle $edgeStyle = null,
                                TextStyle $textStyle = null,
                                $xOffset = 5, $yOffset = 10, $xPadding = 40, $yPadding = 40,
                                $unit = 'px', $orientation = TreeStyle::CENTERED)
    {
        $this->setTreeOrientation($orientation);

        $this->shapeStyle   = $shapeStyle === null ? new ShapeStyle() : $shapeStyle;

        $this->edgeStyle    = $edgeStyle === null ? new EdgeStyle() : $edgeStyle;

        $this->textStyle    = $textStyle === null ?  new TextStyle('Times', 12, new \ws\loewe\Utils\Color\RgbColor(0, 0, 0)) : $textStyle;

        $this->xOffset      = $xOffset;
        $this->yOffset      = $yOffset;

        $this->xPadding     = $xPadding;
        $this->yPadding     = $yPadding;

        $this->unit         = $unit;
        $this->orientation  = $orientation;
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
     * This method sets the tree orientation.
     *
     * @param int $orientation the new tree orientation, one of self::LEFT_BOUND, self::CENTERED, self::RIGHT_BOUND
     */
    public function setTreeOrientation($orientation)
    {
        if($orientation >= self::LEFT_BOUND && $orientation <= self::RIGHT_BOUND)
            $this->orientation = $orientation;

        else
            throw new \InvalidArgumentException ('unsupported tree layout given');
    }

    /**
     * This method sets the x-offset.
     *
     * @param int $xOffset the new x-offset
     */
    public function setXOffset($xOffset)
    {
        $this->xOffset = $xOffset;
    }

    /**
     * This method sets the y-offset.
     *
     * @param int $yOffset the new y-offset
     */
    public function setYOffset($yOffset)
    {
        $this->yOffset = $yOffset;
    }

    /**
     * This method sets the x-padding.
     *
     * @param int $xPadding the new x-padding
     */
    public function setXPadding($xPadding)
    {
        $this->xPadding = $xPadding;
    }

    /**
     * This method sets the y-padding.
     *
     * @param int $yPadding the new y-padding
     */
    public function setYPadding($yPadding)
    {
        $this->yPadding = $yPadding;
    }
}