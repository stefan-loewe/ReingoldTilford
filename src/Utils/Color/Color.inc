<?php

namespace Utils\Color;

/**
 * This class serves as abstract class for any color.
 */
abstract class Color
{
    /**
     * This method is the constructor of the class.
     *
     * @return void
     */
    public function __construct()
    {
    }

    /**
     * This is a magic getter for the members of this class.
     *
     * @param string the name of the member to get
     * @return mixed the respective member
     * @todo this should not be in an abstract class, as it implies this functionality for all of its subclasses
     * @todo replace this with Trait in subclasses
     */
    public function __get($memberName)
    {
        return $this->$memberName;
    }
}