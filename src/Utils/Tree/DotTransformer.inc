<?php

namespace Utils\Tree;

class DotTransformer
{
    private $dotFileName    = null;

    private $nodes          = null;

    public function __construct($fileName)
    {
        $this->dotFileName = $fileName;

        $this->nodes = new \ArrayObject();
    }

    public function transform()
    {
echo "\n".date("Y.m.d H:i:s")." - creating tree data structure ...";

        $contents   = file_get_contents($this->dotFileName);

        $root       = null;

        foreach(explode(PHP_EOL, $contents) as $line)
        {
            if(strpos($line, 'node ') === 0)
            {
                $node = $this->createNode($line);
                if($root === null)
                    $root = $node;
            }

            else if(strpos($line, ' -> ') != FALSE)
                $this->addChildNode($line);
        }

echo "\n".date("Y.m.d H:i:s")." - ... done";
echo "\n".'created '.$this->nodes->count()." nodes";
        return $root;
    }

    private function createNode($nodeString)
    {
        $id     = substr($nodeString, strpos($nodeString, ']') + 2, strlen($nodeString) - strpos($nodeString, ']') - 3);
        $label  = substr($nodeString, strpos($nodeString, 'label=') + 6, strpos($nodeString,  'id="') - (strpos($nodeString, 'label=') + 6) - 1);

        if($id !== null && $label !== null)
        {
            $this->nodes[$id] = new DefaultTreeNode($label);

            return $this->nodes[$id];
        }
        else
            return null;
    }

    private function addChildNode($edgeString)
    {
        $edgeString = substr($edgeString, 0, strpos($edgeString, ' ['));

        $nodeIDs = explode(' -> ', $edgeString);

        $parent = $this->nodes[trim($nodeIDs[0])];
        $child  = $this->nodes[trim($nodeIDs[1])];

        if($child->getParent() != null)
        {
            $_POST[] = array($parent, $child);
            return;
        }

        $parent->appendChild($child);
    }
}
