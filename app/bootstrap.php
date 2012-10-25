<?php

// path to the root folder of the installation
define('INSTALLATION_FOLDER', str_replace('\\', '/', realpath(__DIR__.'/..')));

// path to the source folder
define('SOURCE_FOLDER', INSTALLATION_FOLDER.'/src');

// path to the data folder
define('DATA_FOLDER', INSTALLATION_FOLDER.'/data');

// load the class of the autoloader statically
require_once SOURCE_FOLDER.'/Utils/Autoload/Autoloader.inc';

// instantiate it ...
$autoloader = new Utils\Autoload\Autoloader(SOURCE_FOLDER.'/');

// ... and register it
spl_autoload_register(array($autoloader, 'autoload'));