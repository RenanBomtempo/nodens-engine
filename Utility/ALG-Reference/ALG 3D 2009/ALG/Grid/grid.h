#ifndef GRID_H_
#define GRID_H_

#include "Cell/cell.h"
#include "../Exceptions/invalidParameterValue.h"
#include "../Exceptions/nullPointer.h"

class Grid {
	private:
		CellNode *firstCell;     // First cell of grid.
		float sideLength;        // Length of cube grid. Default = 1.0.
		long int numberOfCells;  // Number of cells of grid.

		// function  for adjustment of cells numbering
		void orderGridCells();

		// Functions related to cell refinement.
		void refineCell( CellNode * ) throw(NullPointer);
		void simplifyGraph( TransitionNode * ) throw(NullPointer);

		// Functions related to cell derefinement.
		void derefineCell( CellNode *) throw(NullPointer);
		void derefineBunch( CellNode * ) throw(NullPointer);
		void clean( TransitionNode * ) throw(NullPointer);
		int getFatherBunchNumber( CellNode * ) throw(NullPointer);
		bool needsDerefinement( CellNode *, float ) throw(NullPointer);
		CellNode *getFrontNortheastCell( CellNode * ) throw(NullPointer);

		// Functions related to cell flux.
		void setCellFlux( CellNode *, char ) throw(NullPointer, InvalidParameterValue);

	public:
		Grid();
		virtual ~Grid();

		// Functions related to drawing and printing.
		void drawGrid( int );
		void drawGraph( int, int, int, int );
		void drawDomain( int );
		void drawHilbertCurve( int );
		void drawCellsNumber( int, void * );
		void drawTransitionNodesDirection( int, void * );
		void print();

		// Functions related to grid refinement.
		void refine( int, float ) throw(InvalidParameterValue);
		void refineCellAt( int )  throw(InvalidParameterValue);
		void refineAll( );

		// Functions related to grid derefinement.
		void derefine( float );
		void derefineAll();
		void derefineFirstBunch();
		void derefineInactiveCells();

		// Functions related to grid domain.
		void setSphericalDomain( float, float )   throw(InvalidParameterValue);
		void setCylindricalDomain( float, float ) throw(InvalidParameterValue);
		void setConicalDomain();
		void setParabolicalDomain();
		void setCubicalDomain();

		// Function used in adjusting grid flux.
		void setFlux() throw(InvalidParameterValue);

		void setTopPlane( float top );

		// Returns the number of CellNodes.
		long int getNumberOfCells();
};

#endif
