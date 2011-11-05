<?php

namespace Utils\Font;

use Utils\Graphics2D\Shapes\Styles\TextStyle;

/**
 * This class encapsulates font metrics, i.e. for a given font family and font size given as text stlye, it can calculate the width and height of a string or a collection of strings.
 */
class FontMetric
{
    /**
     * the TextStyle the FontMetric is based on
     *
     * @var TextStyle
     */
    private $textStyle  = null;

    /**
     * the metrics of the TextStyle, i.e. a mapping from the ASCII code of the TextStyle to the width of the respective character
     *
     * @var ArrayObject
     */
    private $metrics    = null;

    /**
     * This acts as the constructor of the class.
     *
     * @param TextStyle $textStyle the text style to use for the metric
     * @param int[] $charMetrics the metrics of the text style, i.e. a mapping from the ASCII code of the TextStyle to the width of the respective character
     */
    public function __construct(TextStyle $textStyle, $charMetrics)
    {
        $this->textStyle = $textStyle;

        $this->metrics = new \ArrayObject($charMetrics);
    }

    /**
     * This method calculates the width of a string in pixel.
     *
     * @param string $string the string whose width has to be calculated
     * @return int the width of the string in pixels
     */
    public function getWidth($string)
    {
        $width = 0;

        for($i = 0; $i < strlen($string); $i++)
        {
            $char   = $string[$i];

            $width  = $width + $this->metrics[ord($char)];
        }

        return $width;
    }

    /**
     * This method calculates the maximal width of a string in a collection of strings.
     *
     * @param \Traversable $strings the collection of strings
     * @return int the maximal width of the strings in the collection
     */
    public function getMaxWidth(\Traversable $strings)
    {
        $maxWidth = 0;

        foreach(self::splitToLines($strings) as $string)
        {
            if(($currentWidth = $this->getWidth($string)) > $maxWidth)
                $maxWidth = $currentWidth;

        }

        return $maxWidth;
    }

    /**
     * This method calculates the height of a string in pixel.
     *
     * @param string $string the string whose height has to be calculated
     * @return int the height of the string in pixels
     */
    public function getHeight($string)
    {
        if($string === null)
            return 0;

        return (substr_count($string, \PHP_EOL) + 1) * $this->textStyle->getLineHeight();
    }

    /**
     * This method calculates the maximal height of a string in a collection of strings.
     *
     * @param \Traversable $strings the collection of strings
     * @return int the maximal height of the strings in the collection
     */
    public function getMaxHeight(\Traversable $strings)
    {
        $maxHeight = 0;

        foreach($strings as $string)
        {
            if(($currentHeight = $this->getHeight($string)) > $maxHeight)
                $maxHeight = $currentHeight;
        }

        return $maxHeight;
    }

    /**
     * This method return a hash of the FontMetric.
     *
     * @return string the hash of the FontMetric
     */
    public function getHash()
    {
        return $this->textStyle->getHash();
    }

    /**
     * This method splits each string in a collection of strings into a set of strings with no line breaks, and returns all line-break-free strings as ArrayObject.
     *
     * @param \Traversable $strings the collection of string to split
     * @return ArrayObject the collection of strings without line breaks
     */
    private static function splitToLines(\Traversable $strings)
    {
        $lines = new \ArrayObject();

        foreach($strings as $string)
        {
            foreach(explode(\PHP_EOL, $string) as $line)
                $lines[] = $line;
        }

        return $lines;
    }
}