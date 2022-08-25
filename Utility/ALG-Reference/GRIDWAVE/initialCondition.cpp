//initialCondition.cpp

#include "Cell.h"
#include "Grid.h"

/*______________________________________________________________________________
      
                        FUNCTION initialCondition()
                        
   Sets the initial condition for the solution u.
______________________________________________________________________________*/
void initialCondition( Grid *grid )
{
    Cell *gridCell;
    gridCell = grid->firstGridCell;
    
    while( gridCell != 0 )
    {
        gridCell->u = 1.0 * ( 1-16*( (gridCell->centerX - 0.25)*(gridCell->centerX - 0.25)
                    + ( gridCell->centerY - 0.25 ) * ( gridCell->centerY - 0.25 ) ) );
        if( gridCell->u < 0 )
            gridCell->u = 0;
        gridCell->previousU = gridCell->u;
        gridCell = gridCell->next; 
    }
}//end function initialCondition()
//end file initialCondition.cpp

