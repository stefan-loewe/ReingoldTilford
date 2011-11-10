README
======

What is ReingoldTilford?
------------------------

ReingoldTilford is a collection of classes that are capable of visualising any tree-like structure or taxonomy in a number of image formats.

Features
--------

ReingoldTilford supports the following:

* implements the Reingold-Tilford tree drawing algorithm, a liner time (O(n)) algorithm, in pure PHP
* the core algorithm calculates a layout of a n-ary tree-like structure, with which a visualisation of the tree can be created, assuring the following aesthetic criteria:
** obviously, hierarchically and planar connected with straight edges
** the parent is positioned in the middle above its children
** isomorphic subtrees will have the same layout
** drawings of isomorphic subtrees are symmetric with respect to reflection
** in practise, drawings need only a with which is very close to the minimum, or even only the minimal width
* an additional set of classes can export this layout to various formats, e.g. plain HTML, SVG, as well as any format supported by gdlib or Imagick
* the layout and visualisation is highly customizable (shape style, edge style, colours, borders, ports, etc.)
* the package also contains classes to calculate the widths and height of a string of a given font family and size, needed for plotting of node labels etc.
* supporting custom tree-structures is just a matter of creating one class that implements the according interface (ReingoldTilford/Models/ITreeModel)
* when implementing this appropriate model/adapter, classes and libraries that are not available in source form can be made exportable (e.g. PHPs internal DOMNode class, as seen in the examples)
* supporting new formats (like HTML 5 canvas, PDF) is just a matter of creating one class that implements the according interface (Utils/Graphics2D/DrawingPanes/IDrawingPane)
* supporting new shapes (e.g. Triangles as shapes for tree nodes) is just a matter of creating one class that extends the according abstract class (Utils/Graphics2D/Shapes/Shape)
* due to the fact that it supports exporting to DOM documents (HTML, SVG), dynamic manipulation of both the visualized tree as well as the actual data structure is possible with ease
* data structures which are no trees but "almost-tree" can be exported first exporting a tree, then adding the extra edges in a post-processing step
* many more core features* 
* despite not being optimized for speed of execution (but more for ease of extensibility), the package is capable of exporting structures with around 1000 nodes in far less than one second (Intel Pentium T9300 Mobile processor, 4GB RAM, PHP 5.3.8 with Windows CLI), the layout calculation (excluding plotting) for random trees with 100.000 nodes takes around 30 seconds on a AMD Athlon(tm) 64 X2 Dual Core Processor 3800+ with PHP 5.3.2-1ubuntu4.9 with Suhosin-Patch (cli) on Ubuntu 10.10

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