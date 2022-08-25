/**
 * @file grid_refinement.cpp
 * File where all methods of class Grid which are related to refinement are
 * implemented.
 */

#include "grid.h"
#include <iostream>

using std::cout;
using std::endl;

/**
 * Decides if the grid should be refined by traversing the whole grid, according
 * to parameters refinementLevel and refinementBound. A cell will not be refined
 * either if its refinement level  is  equal  to refinementLevel or the  highest
 * of  all  fluxes  coming  into  it  from  the  six  directions  is  less  than
 * refinementBound.
 *
 * @param refinementLevel Minimum refinement level required for the graph.
 * @param refinementBound Minimum flux required for each cell of graph.
 * @throw InvalidArgumentValue If the refinement level argument is not positive,
 * an InvalidArgumentValue exception is thrown.
 */
bool Grid::refine(  int refinementLevel, double refinementBound, int &numRefined ) throw(InvalidArgumentValue) {
	try {
		if( refinementLevel < 0 ) {
			throw InvalidArgumentValue
			("Grid::refine(): Parameter refinementLevel must be positive.", refinementLevel);
		}

		CellNode *gridCell,
		*auxiliarGridCell;
		double maximumFlux;
		bool continueRefining = true;
		bool refinedOnce = false;
		setFlux();

		numRefined = 0;
		while( continueRefining ) {
			continueRefining = false;
			gridCell = firstCell;
			while( gridCell != 0 ) {
				maximumFlux = gridCell->getMaximumFlux();

				if( ( gridCell->level < refinementLevel ) &&
						( maximumFlux >= refinementBound ) )
				{
					//std::cout << maximumFlux << std::endl;
					auxiliarGridCell = gridCell;
					gridCell = gridCell->next;
					refineCell( auxiliarGridCell );
					numberOfCells += 7;
					continueRefining = true;
					refinedOnce = true;
					numRefined += 1;
				}
				else
					gridCell = gridCell->next;
			}
		}
		//orderGridCells();
		return refinedOnce;
	}
	catch( Exception &error ) {
		error.pushIntoStackTrace("Grid::refine()");
		throw;
	}
}

/**
 * Traverses the whole grid refining each cell one level above the refinement
 * level of the cell being refined.
 */
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

/**
 * Refines a cell whose position in the hilbert curve is given by cellNumber.
 *
 * @param cellNumber Position of the cell in the Hilbert's curve.
 * @throw InvalidArgumentValue If the argument cellNumber is neither positive
 * nor less than total number of cells, an InvalidArgumentValue exception is
 * thrown.
 */
void Grid::refineCellAt( int cellNumber ) throw(InvalidArgumentValue) {
	try {
		if( (cellNumber > numberOfCells) || (cellNumber < 0) ) {
			throw InvalidArgumentValue
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
