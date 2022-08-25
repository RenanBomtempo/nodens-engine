/**
 * @file grid_flux.cpp
 * File where the method setFlux(), which is responsible for computing the flux
 * of the whole grid, is implemented.
 */
#include "grid.h"

/**
 * Traverses the grid, computing the fluxes in all directions (north, south,
 * east, west, front, back) due to all neighboring cells in order to compute
 * the total flux.
 */
void Grid::setFlux( ) {
	CellNode *gridCell;

	gridCell = firstCell;
	while ( gridCell != 0 ) {
		gridCell->northFlux = 0.0;
		gridCell->southFlux = 0.0;
		gridCell->eastFlux  = 0.0;
		gridCell->westFlux  = 0.0;
		gridCell->frontFlux = 0.0;
		gridCell->backFlux  = 0.0;

		gridCell = gridCell->next;
	}

	try {
		gridCell = firstCell;
		while ( gridCell != 0 )	{
			gridCell->setCellFlux( 's' ); // Computes south flux.
			gridCell->setCellFlux( 'n' ); // Computes north flux.
			gridCell->setCellFlux( 'e' ); // Computes east flux.
			gridCell->setCellFlux( 'w' ); // Computes west flux.
			gridCell->setCellFlux( 'f' ); // Computes front flux.
			gridCell->setCellFlux( 'b' ); // Computes back flux.

			gridCell = gridCell->next;
		}
	}
	catch( InvalidArgumentValue &error ) {
		   error.pushIntoStackTrace("Grid::setFlux()");
		   throw;
 	}
}
