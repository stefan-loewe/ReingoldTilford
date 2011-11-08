README
======

What is ReingoldTilford?
------------------------

ReingoldTilford is a collection of classes that are capable of visualising any tree-like structure or taxonomy in a number of image formats.

Features
--------

ReingoldTilford supports the following:

ReingoldTilford is a collection of classes that are capable of visualising any tree-like structure or taxonomy in a number of image formats.

ReingoldTilford supports the following:

* implements the Reingold-Tilford tree drawing algorithm, a liner time (O(n)) algorithm, in pure PHP
* the core algorithm calculates a layout of a tree-like structure, with which a visualisation of the tree can be created
* an additional set of classes can export this layout to various formats, e.g. plain HTML, SVG, as well as any format supported by gdlib or Imagick
* the layout and visiualisation is highly customizeable (shape style, edge style, colors, borders, etc.)
* the package also contains classes to calculate the widths and height of a string of a given font family and size, needed for plotting of node labels etc.
* supporting custom tree-structures is just a matter of creating one class that implements the according interface (ReingoldTilford/Models/ITreeModel)
* when implementing this appropriate model/adapter, classes and libraries that are not available in source form can be made exportable (e.g. PHPs internal DOMNode class, as seen in the examples)
* supporting new formats (like HTML 5 canvas, PDF) is just a matter of creating one class that implements the according interface (Utils/Graphics2D/DrawingPanes/IDrawingPane)
* supporting new shapes (e.g. Triangles as shapes for tree nodes) is just a matter of creating one class that extends the according abstract class (Utils/Graphics2D/Shapes/Shape)
* due to the fact that it supports exporting to DOM documents (HTML, SVG), dynamic manipulation of both the visualized tree as well as the actual data structure is possible with ease
* many more core features
* despite not being optimized for speed of execution (but more for ease of extensibility), the package is capable of exporting structures with around 1000 nodes in under one second (Intel Pentium T9300 Mobile processor, 4GB RAM, PHP 5.3.8 with Windows CLI)

Requirements
------------

ReingoldTilford requires the following:

* PHP 5.3.X, due to namespace support
* PHP DOM extension (optional, needed fro HTML and SVG export)
* PHP GD Graphics Library (optional, needed for GIF/JPG/PNG export with php_gd2 extension)
* PHP ImageMagick (optional, needed for export with php_imagick extension)

Installation
------------

Extract the files to any path in your web root directory.

How to use ReingoldTilford?
-------------------

Point your web browser to the examples in the bin folder: 

Visualisation of the directory structure of this package
- http://localhost/ReingoldTilford/bin/example_directory_tree.php

Visualisation of the DOM structure of http://www.phpclasses.org/ (or any other website if you customize the example)
- http://localhost/ReingoldTilford/bin/example_DOM.php