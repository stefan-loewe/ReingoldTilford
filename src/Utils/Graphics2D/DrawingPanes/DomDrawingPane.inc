<?php

namespace Utils\Graphics2D\DrawingPanes;

use Utils\Color\RgbColor;
use Utils\Geom\Dimension;

/**
 * This class implements an abstract drawing pane for DOM output.
 */
abstract class DomDrawingPane extends DrawingPane
{
    /**
     * the DOM document fragment that holds the source code of the export
     *
     * @var DOMDocumentFragment
     */
    protected $fragment         = null;

    /**
     * the name of the CSS class for the node shapes.
     *
     * @var string
     */
    protected $classNameNode    = 'rt_node';

    /**
     * the name of the CSS class for the edge shapes.
     *
     * @var string
     */
    protected $classNameEdge    = 'rt_edge';

    /**
     * This method acts as the constructor of the class.
     *
     * @param Dimension $dimension the dimension of the document
     */
    public function __construct(Dimension $dimension)
    {
        parent::__construct($dimension);

        $this->document = new \DOMDocument();

        $this->fragment = $this->document->createDocumentFragment();
    }

    /**
     * This method assures that the document is empty before saving it. Otherwise, multiple calls to violate the DOMDocument structure (e.g. multiple <svg elements>). Creating a new document does not work, as the fragement depends on the first created document.
     */
    protected function clearDocument()
    {
        foreach($this->document->childNodes as $child)
            $this->document->removeChild ($child);
    }

    /**
     * This method appends the fragment to the given parent element.
     *
     * @param \DOMNode $parentElement the root element to append the internal document fragment to
     */
    protected function appendFragment(\DOMNode $parentElement)
    {
        // calling cloneNode, otherwise, multiple calls to save would not be possible
        // as appendChiild moves all nodes from fragement to document (leaving the fragment empty)
        $parentElement->appendChild($this->fragment->cloneNode(TRUE));
    }

    /**
     * This method acts as getter for the CSS class name of the node shapes.
     *
     * @return string
     */
    public function getClassNameNode()
    {
        return $this->classNameNode;
    }

    /**
     * This method acts as getter for the CSS class name of the edge shapes.
     *
     * @return string
     */
    public function getClassNameEdge()
    {
        return $this->classNameEdge;
    }

    /**
     * This method acts as setter for the CSS class name of the node shapes.
     *
     * @param string $classNameNode the new name of the CSS class for node shapes
     * return void
     */
    public function setClassNameNode($classNameNode)
    {
        $this->classNameNode = classNameNode;
    }

    /**
     * This method acts as setter for the CSS class name of the edge shapes.
     *
     * @param string $classNameEdge the new name of the CSS class for edge shapes
     * return void
     */
    public function setClassNameEdge($classNameEdge)
    {
        $this->classNameEdge = $classNameEdge;
    }

    /**
     * This method converts a RGBColor to the DOM representation of the color.
     *
     * @param RGBColor $color the color to convert
     * @return string the DOM representation of the color
     */
    public function toDOMColor(RGBColor $color)
    {
        return $color->__toString();
    }
}