/* Element.h
Definition of class Element.
Element is a linked list of entries that will play the role of a row in the grid
discretization matrix associated to this cell.
For performance reasons (i.e., in order to avoid multiple function calls),
all members are declared public.
*/

#ifndef ELEMENT_H
#define ELEMENT_H

#include "Cell.h"

class Cell;

class Element
{
    public:
        
        Element();			//Default constructor.
        ~Element();			//Destructor.

        double value;        
        int column;			//Column of the matrix to which this element belongs.
        Element *next;      //Next element in the list.
        Cell *cell;         //Cell to which this element's row is associated.
        
};//end class Element

#endif
