<?php

namespace Utils\Font\FontMetricServices;

use Utils\Font\FontManager;
use Utils\Graphics2D\Shapes\Styles\TextStyle;
use Utils\File\Directory;
use \Utils\URL\Url;

/**
 * This class represents an abstract front metric service.
 */
abstract class FontMetricService
{
    /**
     * the ASCII code for the blank, i.e. " "
     */
    const ASCII_BLANK = 32;

    /**
     * the file name where the template for the font metric HTML file is stored
     *
     * @var string
     */
    private $fontMetricsTemplate = 'fontMetrics.tpl';

    /**
     * This method acts as the constructor of the class
     */
    public function __construct()
    {
    }

    /**
     * This method creates html code which is either opened as static file (@see FontMetricSocketService) or served dynamically (@see FontMetricWebServerService).
     *
     * The html code of the template is complated, and a span element for each of the first 255 charachters of the current text style are added. The javascript code embedded in the template html code gets the width of each span element (i.e. the width of each character) and sends this information bach to the registered service endpoint.
     *
     * @param TextStyle $textStyle the text style for which the html should be created
     * @param Url $fontMetricServiceEndpoint the service endpoint to which the result should be sent to
     * @return string the generated html code
     */
    protected function createHtml(TextStyle $textStyle, Url $fontMetricServiceEndpoint)
    {
        // load the template ...
        $code = file_get_contents(Directory::toRealPath(__DIR__.'/../'.$this->fontMetricsTemplate));

        // ... set the title, ...
        $code = str_replace('$FONT_DEFINITION', $textStyle->__toString(), $code);

        // ... the location of the backend, ...
        $code = str_replace('$BACKEND', '"'.$fontMetricServiceEndpoint->__toString().'"', $code);

        // ... the style, ...
        $code = str_replace('$STYLE', $textStyle->toCssClass('chr'), $code);

        // ... and the content - a html span element for each of the first 255 characters of the text style
        $content = '';
        for ($i = 0; $i <= 255; $i++)
        {
            $char = ($i === self::ASCII_BLANK) ? '&nbsp;' : htmlentities(chr($i));

            $content = $content.'<span id="c'.$i.'" class="chr">'.$char.'</span>';

            if($i != 255)
                $content = $content.PHP_EOL."\t\t";
        }

        return str_replace('$CONTENT', '<div>'.PHP_EOL.$content.PHP_EOL.'</div>', $code);
    }

    /**
     * This method executes the respective service.
     *
     * @param TextStyle $textStyle the text style for which the font definition is needed
     * @param FontManager $fontManager the font manager to which the font definition is added to
     */
    abstract function execute(TextStyle $textStyle, FontManager $fontManager);
}
