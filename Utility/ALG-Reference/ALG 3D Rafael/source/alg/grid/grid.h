/**
 * @file grid.h
 * File where the class grid is defined.
 */

#ifndef GRID_H_
#define GRID_H_

#include "cell/cell.h"
#include "../exceptions/invalidArgumentValue.h"
#include "../exceptions/nullPointer.h"

/**
 * @class Grid
 * Class that encapsulate the ALG graph.
 */
class Grid {
private:
	CellNode *firstCell;     // First cell of grid.
	double sideLength;        // Length of cube grid. Default = 1.0.
	unsigned long long int numberOfCells;  // Number of cells of grid.
	int numberOfIterations;
	double pError;

	// Time used for solving wave equation.
	double timeInstant;
	double deltaT;

	void initializeGrid();

	// Method for adjustment of cells numbering
	void orderGridCells();


	// Methods related to cell refinement.
	void refineCell( CellNode * ) throw(NullPointer);
	void simplifyRef( TransitionNode * ) throw(NullPointer);

	// Methods related to cell derefinement.
	void derefineCell( CellNode *) throw(NullPointer);
	void derefineBunch( CellNode * ) throw(NullPointer);
	void simplifyDeref( TransitionNode * ) throw(NullPointer);
	int getFatherBunchNumber( CellNode * ) throw(NullPointer);
	bool needsDerefinement( CellNode *, double ) throw(NullPointer);
	CellNode *getFrontNortheastCell( CellNode * ) throw(NullPointer);

	// Method related to cell flux.
	void setFlux( );
	void setCellFlux( CellNode *, char ) throw(NullPointer, InvalidArgumentValue);

	//Method related to Monodomain
	void initializeDiagonalElementsMonodomain(double beta, double Cm, double dt);
	void initialConditionMonodomain(double, double, double, double );
	void setDiscretizationMatrixMonodomain(double beta, double cm, double dt);
	void fillDiscretizationMatrixElementsMonodomain(CellNode *gridCell, Cell *neighborGridCell, char direction);
	void conjugateGradient(int maxIts);
	void solveODEs(double time, double stimStart, double stimDur, double beta, double cm, double dt, double i_stim);
	void setNewSV();
	void setInitalConditionsODEs();
	void printOrdered(string outfile);

public:
	Grid();
	virtual ~Grid();

	// Method related to grid refinement.
	bool refine( int, double, int &) throw(InvalidArgumentValue);
	void refineCellAt( int )  throw(InvalidArgumentValue);
	void refineAll( );

	// Method related to grid derefinement.
	bool derefine( double, int, int & );
	void derefineAll();
	void derefineFirstBunch();
	void derefineInactiveCells();

	// Method related to grid domain.
	void setSphericalDomain( double, double )   throw(InvalidArgumentValue);
	void setCylindricalDomain( double, double ) throw(InvalidArgumentValue);
	void setConicalDomain();
	void setParabolicalDomain();
	void setCubicalDomain();

	// Methods used for solving wave equation.
	void solveMonodomain( int argc, char** argv  );
	long int getNumberOfCells();
	void printGridMatrix();
};

#endif
