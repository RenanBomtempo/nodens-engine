/**
 * @file grid_derefinement.cpp
 * File where all methods of class Grid which are related to derefinement of
 * grid are implemented.
 */

#include "grid.h"
#include <iostream>
/**
 * Decides if the grid should be derefined by traversing the whole grid,
 * according to the parameter derefinementBound.
 *
 * @param derefinementBound Derefinement condition.
 */
bool Grid::derefine( double derefinementBound, int maximumDerefinementeLevel, int &numDerefined ) {

	CellNode *gridCell,
		     *auxiliarGridCell;

	long int bunchNumber1,
		 	 bunchNumber2,
		 	 bunchNumber3,
		 	 bunchNumber4,
			 bunchNumber5,
		 	 bunchNumber6,
		 	 bunchNumber7,
		 	 bunchNumber8;

	bool hasBeenDerefined;
	bool derefinedOnce = false;
	setFlux();
	numDerefined = 0;

	try {
		gridCell = firstCell;
		while( gridCell != 0 ) {
			hasBeenDerefined = false;

			if( gridCell->level >  maximumDerefinementeLevel) {

				/* Checks if the next seven cells of the current cell exist. */
				if( ( gridCell->next != 0 ) &&
					( gridCell->next->next != 0 ) &&
					( gridCell->next->next->next != 0 ) &&
					( gridCell->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next->next->next != 0 ) )
				{
					/* Verifies if each one of the next seven  cells  has  the  same
					 * refinement level. */
					if( ( gridCell->level == gridCell->next->level ) &&
						( gridCell->level == gridCell->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->next->next->level ) )
					{
						/* Checks if this cell and the next seven cells belong to the
						 * same bunch. */
						bunchNumber1 = gridCell->bunchNumber / 10;
						bunchNumber2 = gridCell->next->bunchNumber / 10 ;
						bunchNumber3 = gridCell->next->next->bunchNumber / 10 ;
						bunchNumber4 = gridCell->next->next->next->bunchNumber / 10 ;
						bunchNumber5 = gridCell->next->next->next->next->bunchNumber / 10 ;
						bunchNumber6 = gridCell->next->next->next->next->next->bunchNumber / 10 ;
						bunchNumber7 = gridCell->next->next->next->next->next->next->bunchNumber / 10 ;
						bunchNumber8 = gridCell->next->next->next->next->next->next->next->bunchNumber / 10 ;
						if( (bunchNumber1 == bunchNumber2 ) &&
							(bunchNumber1 == bunchNumber3 ) &&
							(bunchNumber1 == bunchNumber4 ) &&
							(bunchNumber1 == bunchNumber5 ) &&
							(bunchNumber1 == bunchNumber6 ) &&
							(bunchNumber1 == bunchNumber7 ) &&
							(bunchNumber1 == bunchNumber8 ) )
						{
							/* Notice that by the ordering conferred by the  Hilbert
							 * curve, the program always  enters  the  bunch  to  be
							 * derefined by its first member cell.
							 */
							if( needsDerefinement( gridCell, derefinementBound ) )
							{
								auxiliarGridCell =
								gridCell->next->next->next->next->next->next->next->next;

								derefineBunch( gridCell );
								numberOfCells -= 7;
								gridCell = auxiliarGridCell;
								hasBeenDerefined = true;
								derefinedOnce= true;
								numDerefined += 1;
							}
						}
					}
				}
			}
			if( hasBeenDerefined == false )
				gridCell = gridCell->next;
		}
		//orderGridCells();
		return derefinedOnce;
	}
	catch( Exception &error ) {
		   error.pushIntoStackTrace( "Grid::derefine()" );
		   throw;
 	}
}

/**
 * Derefines whole grid one level below the current refinement level.
 */
void Grid::derefineAll() {

	CellNode *gridCell,
		     *auxiliarGridCell;

	long int bunchNumber1,
		 	 bunchNumber2,
		 	 bunchNumber3,
		 	 bunchNumber4,
			 bunchNumber5,
		 	 bunchNumber6,
		 	 bunchNumber7,
		 	 bunchNumber8;

	bool hasBeenDerefined;

	try {
		gridCell = firstCell;
		while( gridCell != 0 ) {
			hasBeenDerefined = false;
			if( gridCell->level >  1) {
				/* Checks if the next seven cells of the current cell exist. */
				if( ( gridCell->next != 0 ) &&
					( gridCell->next->next != 0 ) &&
					( gridCell->next->next->next != 0 ) &&
					( gridCell->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next->next->next != 0 ) )
				{
					/* Verifies if each one of the next seven  cells  has  the  same
					 * refinement level. */
					if( ( gridCell->level == gridCell->next->level ) &&
						( gridCell->level == gridCell->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->next->next->level ) )
					{
						/* Checks if this cell and the next seven cells belong to the
						 * same bunch. */
						bunchNumber1 = gridCell->bunchNumber / 10;
						bunchNumber2 = gridCell->next->bunchNumber / 10 ;
						bunchNumber3 = gridCell->next->next->bunchNumber / 10 ;
						bunchNumber4 = gridCell->next->next->next->bunchNumber / 10 ;
						bunchNumber5 = gridCell->next->next->next->next->bunchNumber / 10 ;
						bunchNumber6 = gridCell->next->next->next->next->next->bunchNumber / 10 ;
						bunchNumber7 = gridCell->next->next->next->next->next->next->bunchNumber / 10 ;
						bunchNumber8 = gridCell->next->next->next->next->next->next->next->bunchNumber / 10 ;
						if( (bunchNumber1 == bunchNumber2 ) &&
							(bunchNumber1 == bunchNumber3 ) &&
							(bunchNumber1 == bunchNumber4 ) &&
							(bunchNumber1 == bunchNumber5 ) &&
							(bunchNumber1 == bunchNumber6 ) &&
							(bunchNumber1 == bunchNumber7 ) &&
							(bunchNumber1 == bunchNumber8 ) )
						{
							/* Notice that by the ordering conferred by the  Hilbert
							 * curve, the program always  enters  the  bunch  to  be
							 * derefined by its first member cell.
							 */
							auxiliarGridCell =
							gridCell->next->next->next->next->next->next->next->next;

							derefineBunch( gridCell );
							numberOfCells -= 7;
							gridCell = auxiliarGridCell;
							hasBeenDerefined = true;
						}
					}
				}
			}
			if( !hasBeenDerefined )
				gridCell = gridCell->next;
		}
		orderGridCells();
	}
	catch( Exception &error ) {
		   error.pushIntoStackTrace( "Grid::derefineAll()" );
		   throw;
 	}
}

/**
 * Derefines the first eighth cells of a grid which belong to the same bunch.
 */
void Grid::derefineFirstBunch( ) {
	CellNode *gridCell,
		 	 *auxiliarGridCell;

	long int bunchNumber1,
		 	 bunchNumber2,
		 	 bunchNumber3,
		 	 bunchNumber4,
			 bunchNumber5,
		 	 bunchNumber6,
		 	 bunchNumber7,
		 	 bunchNumber8;

	bool hasBeenDerefined = false;

	try {
		gridCell = firstCell;
		while(( gridCell != 0 ) && (!hasBeenDerefined))	{
			if( gridCell->level > 1 ) {
				/* Checks if the next seven cells of the current cell exist. */
				if( ( gridCell->next != 0 ) &&
					( gridCell->next->next != 0 ) &&
					( gridCell->next->next->next != 0 ) &&
					( gridCell->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next->next->next != 0 ) )
				{
					/* Verifies if each one of the next seven  cells  has  the  same
					 * refinement level. */
					if( ( gridCell->level == gridCell->next->level ) &&
						( gridCell->level == gridCell->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->next->next->level ) )
					{
						/* Checks if this cell together with the next seven cells
						 * belong to the same bunch. */
						bunchNumber1 = gridCell->bunchNumber / (int)10;
						bunchNumber2 = gridCell->next->bunchNumber / (int)10 ;
						bunchNumber3 = gridCell->next->next->bunchNumber / (int)10 ;
						bunchNumber4 = gridCell->next->next->next->bunchNumber / (int)10 ;
						bunchNumber5 = gridCell->next->next->next->next->bunchNumber / (int)10 ;
						bunchNumber6 = gridCell->next->next->next->next->next->bunchNumber / (int)10 ;
						bunchNumber7 = gridCell->next->next->next->next->next->next->bunchNumber / (int)10 ;
						bunchNumber8 = gridCell->next->next->next->next->next->next->next->bunchNumber / (int)10 ;
						if( (bunchNumber1 == bunchNumber2 ) &&
							(bunchNumber1 == bunchNumber3 ) &&
							(bunchNumber1 == bunchNumber4 ) &&
							(bunchNumber1 == bunchNumber5 ) &&
							(bunchNumber1 == bunchNumber6 ) &&
							(bunchNumber1 == bunchNumber7 ) &&
							(bunchNumber1 == bunchNumber8 ) )
						{
							auxiliarGridCell =
							gridCell->next->next->next->next->next->next->next->next;

							derefineBunch( gridCell );
							numberOfCells -= 7;
							gridCell = auxiliarGridCell;
							hasBeenDerefined = true;
						}
					}
				}
			}
			if( !hasBeenDerefined )
				gridCell = gridCell->next;
		}
		orderGridCells();
	}
	catch( Exception &error ) {
		   error.pushIntoStackTrace( "Grid::derefineFistBunch()" );
		   throw;
 	}
}

/**
 * Derefines all inactive cells of grid.
 */
void Grid::derefineInactiveCells( ) {
	CellNode *gridCell,
		 	 *auxiliarGridCell;

	long int bunchNumber1,
		 	 bunchNumber2,
		 	 bunchNumber3,
		 	 bunchNumber4,
			 bunchNumber5,
		 	 bunchNumber6,
		 	 bunchNumber7,
		 	 bunchNumber8;

	bool active1,
		 active2,
		 active3,
		 active4,
		 active5,
		 active6,
		 active7,
		 active8;

	bool hasBeenDerefined;

	try {
		gridCell = firstCell;
		while( gridCell != 0 ) {
			hasBeenDerefined = false;

			if( gridCell->level >  1)
			{
				/* Checks if the next seven  cells of the current cell exist. */
				if( ( gridCell->next != 0 ) &&
					( gridCell->next->next != 0 ) &&
					( gridCell->next->next->next != 0 ) &&
					( gridCell->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next->next != 0 ) &&
					( gridCell->next->next->next->next->next->next->next != 0 ) )
				{
					/* Verifies if each one of the next seven  cells  has  the  same
					 * refinement level. */
					if( ( gridCell->level == gridCell->next->level ) &&
						( gridCell->level == gridCell->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->next->level ) &&
						( gridCell->level == gridCell->next->next->next->next->next->next->next->level ) )
					{
						/* Checks if this cell together with the next seven cells
						 * belong to the same bunch. */
						bunchNumber1 = gridCell->bunchNumber / 10;
						bunchNumber2 = gridCell->next->bunchNumber / 10 ;
						bunchNumber3 = gridCell->next->next->bunchNumber / 10 ;
						bunchNumber4 = gridCell->next->next->next->bunchNumber / 10 ;
						bunchNumber5 = gridCell->next->next->next->next->bunchNumber / 10 ;
						bunchNumber6 = gridCell->next->next->next->next->next->bunchNumber / 10 ;
						bunchNumber7 = gridCell->next->next->next->next->next->next->bunchNumber / 10 ;
						bunchNumber8 = gridCell->next->next->next->next->next->next->next->bunchNumber / 10 ;
						if( (bunchNumber1 == bunchNumber2 ) &&
							(bunchNumber1 == bunchNumber3 ) &&
							(bunchNumber1 == bunchNumber4 ) &&
							(bunchNumber1 == bunchNumber5 ) &&
							(bunchNumber1 == bunchNumber6 ) &&
							(bunchNumber1 == bunchNumber7 ) &&
							(bunchNumber1 == bunchNumber8 ) )
						{
							/* Verifies if each one of the next seven cells has  are
							 * inactive */
							active1 = gridCell->active;
							active2 = gridCell->next->active;
							active3 = gridCell->next->next->active;
							active4 = gridCell->next->next->next->active;
							active5 = gridCell->next->next->next->next->active;
							active6 = gridCell->next->next->next->next->next->active;
							active7 = gridCell->next->next->next->next->next->next->active;
							active8 = gridCell->next->next->next->next->next->next->next->active;
							if( ( !active1 ) && ( !active2 ) && ( !active3 ) && ( !active4 )
							 && ( !active5 ) && ( !active6 ) && ( !active7 ) && ( !active8 ) )
							{
								auxiliarGridCell =
								gridCell->next->next->next->next->next->next->next->next;

								derefineBunch( gridCell );
								numberOfCells -= 7;
								gridCell = auxiliarGridCell;
								hasBeenDerefined = true;
							}
						}
					}
				}
			}
			if( !hasBeenDerefined )
				gridCell = gridCell->next;
		}
		orderGridCells();
	}
	catch( Exception &error ) {
		   error.pushIntoStackTrace( "Grid::derefineInactiveCell()" );
		   throw;
 	}
}
