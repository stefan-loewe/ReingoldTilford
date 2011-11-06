<?php

define('INSTALLATION_FOLDER', str_replace('\\', '/', realpath(__DIR__.'/..')));

define('SOURCE_FOLDER', INSTALLATION_FOLDER.'/src');

require_once SOURCE_FOLDER.'/Utils/Autoload/Autoloader.inc';

$autoloader = new Utils\Autoload\Autoloader(SOURCE_FOLDER.'/');

spl_autoload_register(array($autoloader, 'autoload'));