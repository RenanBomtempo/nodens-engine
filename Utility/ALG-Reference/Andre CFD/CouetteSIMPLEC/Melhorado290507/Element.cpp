/* Element.cpp
Implementation of class Element. */

#include "Element.h"
#include "Cell.h"

//Constructor
Element::Element():
    value(0.0),
    column(0),
    next(0),
    cell(0)
{}

//Destructor
Element::~Element()
{}
