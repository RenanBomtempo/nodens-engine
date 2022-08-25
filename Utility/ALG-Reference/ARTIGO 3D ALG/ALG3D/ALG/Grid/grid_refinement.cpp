#include "grid.h"
#include <iostream>

using std::cout;
using std::endl;

/*==============================================================================
                               FUNCTION refineGrid()

    Decides if the grid should be refined by traversing the whole grid, according
    to the parameters refinementLevel and refinementBound.
    A cell will not be refined either if its level of refinement is equal to
    refinementLevel or the highest of all fluxes coming into it from the six
    directions is less than refinementBound.
==============================================================================*/
void Grid::refine(  int refinementLevel, float refinementBound ) throw(InvalidParameterValue) {
	try {
		if( refinementLevel < 0 ) {
			throw InvalidParameterValue
			("Grid::refine(): Parameter refinementLevel has to be positive.", refinementLevel);
		}
	
		CellNode *gridCell,
		          *auxiliarGridCell;
		float maximumFlux;
		bool continueRefining = true;
		setFlux();
		
		gridCell = firstCell;
		while( continueRefining ) {
			continueRefining = false;
			gridCell = firstCell;
			while( gridCell != 0 ) {
				//Computes maximum flux coming to this cell.
				maximumFlux = gridCell->getMaximumFlux();
				if( ( gridCell->level < refinementLevel ) &&
					( maximumFlux >= refinementBound ) )
				{
					auxiliarGridCell = gridCell;
					gridCell = gridCell->next;
					refineCell( auxiliarGridCell );
					numberOfCells += 7;
			
					continueRefining = true;
				}
				else
					gridCell = gridCell->next;
			}
		}
		orderGridCells();
	}
	catch( Exception &error ) {
		error.pushIntoStackTrace("Grid::refine()");
		throw;
 	}
}

/*==============================================================================
                             FUNCTION refineAll()
                         
    Refines whole grid one level above the current level of each cell.
==============================================================================*/
void Grid::refineAll() {
    try { 
    	CellNode *gridCell,
    		      *auxiliarGridCell;
    
    	gridCell = firstCell;
    	while ( gridCell != 0 )	{
    		auxiliarGridCell = gridCell;
    		gridCell = gridCell->next;
    		refineCell( auxiliarGridCell );
    		numberOfCells += 7;
    	}
    	orderGridCells();
    }
    catch( Exception &error ) {
        error.pushIntoStackTrace("Grid::refineAll()");
		throw;
    }
}

/*==============================================================================
                           FUNCTION refineCellAt()
                         
    Refines a specific cell of the grid. Its parameter is the number of the cell
	in the Hilbert's curve.
==============================================================================*/
void Grid::refineCellAt( int cellNumber ) throw(InvalidParameterValue) {
 	try {	
		if( (cellNumber > numberOfCells) || (cellNumber < 0) ) {
			throw InvalidParameterValue
			("Grid::refineCellAt(): cellNumber is out of bounds.", cellNumber);	
		}
		 
		CellNode *gridCell = firstCell;
		for( int i = 0; i < cellNumber; i++ ) {
			 gridCell = gridCell->next;
	 	}
	 	
		refineCell( gridCell );
		numberOfCells += 7;
		orderGridCells();
	}
	catch( Exception &error ) {
		error.pushIntoStackTrace("Grid::refineCellAt()");
		throw;
 	}
}
