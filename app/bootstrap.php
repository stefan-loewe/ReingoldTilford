<?php

// path to the root folder of the installation
define('INSTALLATION_FOLDER', str_replace('\\', '/', realpath(__DIR__.'/..')));

// path to the source folder
define('SOURCE_FOLDER', INSTALLATION_FOLDER.'/src');

// path to the data folder
define('DATA_FOLDER', INSTALLATION_FOLDER.'/data');

// load the class of the autoloader statically
require_once INSTALLATION_FOLDER.'/../Utils/src/ws/loewe/Utils/Autoload/Autoloader.php';

// instantiate it ...
$autoloaderRT = new \ws\loewe\Utils\Autoload\Autoloader(SOURCE_FOLDER.'/', 'php');
$autoloaderUtils = new \ws\loewe\Utils\Autoload\Autoloader(INSTALLATION_FOLDER.'/../Utils/src/', 'php');

// ... and register it
spl_autoload_register(array($autoloaderRT, 'autoload'));
spl_autoload_register(array($autoloaderUtils, 'autoload'));