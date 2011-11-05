<?php

namespace Utils\Font;

use Utils\Graphics2D\Shapes\Styles\TextStyle;
use Utils\Font\FontMetric;
use Utils\Font\FontMetricServices\FontMetricService;

/**
 * This class is responsible for managing font metrics.
 */
class FontManager
{
    /**
     * the collection containing the FontMetrics
     *
     * @var ArrayObject
     */
    private $fontMetrics            = null;

    /**
     * the FontMetricService that is in use to get FontMetrics out of TextStyles
     *
     * @var FontMetricService
     */
    private $fontMetricService      = null;

    /**
     * the file name where FontMetrics are cached
     *
     * @var string
     */
    private $fontMetricsFileName    = 'fontMetrics.ser';

    /**
     * This method acts as the constructor of the class,
     *
     * @param FontMetricService $fontMetricService the FontMetricService to use
     */
    public function __construct(FontMetricService $fontMetricService = null)
    {
        $this->initializeFontMetrics();

        $this->fontMetricService = $fontMetricService;
    }

    /**
     * This method initialises the internal FontMetrics data structure, i.e. loading FontMetrics from the cache, if a cache file exists.
     *
     * @return boolean true, if a cache for FontMetrics was found, else false
     */
    private function initializeFontMetrics()
    {
        $this->fontMetrics = new \ArrayObject();

        $filename = $this->getFontMetricsPath();

        if(file_exists($filename))
        {
            $this->fontMetrics = unserialize(file_get_contents($filename));

            return true;
        }

        return false;
    }

    /**
     * This method creates a FontMetric for a given TextStyle on behalf of the used FontMetricService.
     *
     * @param TextStyle $textStyle the TextStyle for which a FontMetric has to be created
     */
    private function createFontMetric(TextStyle $textStyle)
    {
        if(!$this->fontMetrics->offsetExists($textStyle->getHash()))
            $this->fontMetricService->execute($textStyle, $this);
    }

    /**
     * This method returns the full path to the file where FontMetrics are cached persistently.
     *
     * @return string the full path to the FontMetrics cache file
     */
    private function getFontMetricsPath()
    {
        return __DIR__.DIRECTORY_SEPARATOR.$this->fontMetricsFileName;
    }

    /**
     * This method adds a FontMetric to the FontManager.
     *
     * @param FontMetric $metric the FontMetric to be added
     */
    public function addFontMetric(FontMetric $metric)
    {
        $this->fontMetrics[$metric->getHash()] = $metric;

        file_put_contents($this->getFontMetricsPath(), serialize($this->fontMetrics));
    }

    /**
     * This method returns the FontMetric for a given TextStyle.
     *
     * If the FontMetric for the given TextStyle is not yet created, the FontMetricService is being called, to first create it.
     *
     * @param TextStyle $textStyle the TextStyle whose FontMetric has to be retrieved
     * @return FontMetric the FontMetric for the given TextStyle
     */
    public function getFontMetric(TextStyle $textStyle)
    {
        $hash = $textStyle->getHash();

        if(!$this->fontMetrics->offsetExists($hash))
        {
            $this->createFontMetric($textStyle);

            // when creating font metrics via web interface, they might not be ready
            // yet, so wait for a bit
            for($i = 0; $i < 1000; $i++)
            {
                if($this->initializeFontMetrics())
                    break;
                else
                    usleep(1000);
            }
            sleep(3);
        }

        return $this->fontMetrics[$hash];
    }
}