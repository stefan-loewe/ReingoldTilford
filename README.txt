README
======

What is ReingoldTilford?
------------------------

ReingoldTilford is a collection of classes that are capable of visualising any tree-like structure or taxonomy in a number of image formats.

Features
--------

ReingoldTilford supports the following:

* implements the Reingold-Tilford tree drawing algorithm, a liner time (O(n)) algorithm, in pure PHP
* the core algorithm calculate a layout of a tree-like structure, with which a visualisation of the tree can be created
* an additional set of classes can export this layout to various formats, e.g. plain HTML, SVG, as well as any format supported by gdlib or Imagick
* the layout and visiualisation is highly customizeable (shape style, edge style, colors, borders, etc.)                                              
* supporting custom tree-structures is just a matter of creating one class that implements the according interface (ReingoldTilford/Models/ITreeModel)
* supporting new formats (like HTML 5 canvas, PDF) is just a matter of creating one class that implements the according interface (Utils/Graphics2D/DrawingPanes/IDrawingPane)
* supporting new shapes (e.g. Triangles as shapes for tree nodes) is just a matter of creating one class that extends the according abstract class (Utils/Graphics2D/Shapes/Shape)
* capable of exporting structures with around 1000 nodes in under one second (Intel Pentium T9300 Mobile processor, 4GB RAM, PHP 5.3.8 with Windows CLI)

Requirements
------------

ReingoldTilford requires the following:

* PHP 5.3.X
* PHP DOM extension (optional, but enabled by default anyway)
* PHP GD Graphics Library (optional)
* PHP ImageMagick (optional)

Installation
------------

Extract the files to any path in your web root directory.

How to use ReingoldTilford?
-------------------

Point your web browser to the examples in the bin folder: 

Visualisation of the directory structure of this package
- http://localhost/ReingoldTilford//bin/example_directory_tree.php

Visualisation of the DOM structure of http://www.phpclasses.org/ (or any other website if you customize the example)
- http://localhost/ReingoldTilford/bin/example_DOM.php