/**
 * @file cell_refinement.cpp
 * File where all methods of class CellNode which are related to cell refinement
 * are implemented.
 */
#include "../grid.h"
#include <cstdio>
#include <cstring>
using namespace std;

/**
 * Refines a cell, transforming it into a bunch with eight cell nodes and six
 * transition nodes.
 *
 * @param cell Cell node to refine.
 * @throw NullPointer If a null cell node is given as argument, a NullPointer
 * exception is thrown.
 */
void Grid::refineCell( CellNode *cell ) throw (NullPointer) {
	if( cell == 0 ) {
		throw NullPointer
		("Grid::refineCell(): Parameter cell is NULL.");
	}

	TransitionNode *eastTransitionNode,
	*northTransitionNode,
	*westTransitionNode,
	*southTransitionNode,
	*frontTransitionNode,
	*backTransitionNode;

	CellNode *frontNortheastSubCell,
	*frontNorthwestSubCell,
	*frontSouthwestSubCell,
	*frontSoutheastSubCell,
	*backNortheastSubCell,
	*backNorthwestSubCell,
	*backSouthwestSubCell,
	*backSoutheastSubCell;

	int numberOfHilbertShape;

	double cellCenterX     = cell->centerX,
			cellCenterY     = cell->centerY,
			cellCenterZ     = cell->centerZ,
			cellHalfSide    = cell->halfFaceLength,
			cellQuarterSide = cell->halfFaceLength / 2.0;

	long int oldBunchNumber = cell->bunchNumber;

	// Creation of the front northeast cell. This cell, which is to be refined,
	// becomes the frontNortheast cell of the new bunch.
	frontNortheastSubCell                 = cell;
	frontNortheastSubCell->level          = cell->level + 1;
	frontNortheastSubCell->faceLength     = cellHalfSide;
	frontNortheastSubCell->halfFaceLength = cellQuarterSide;
	frontNortheastSubCell->centerX        = cellCenterX + cellQuarterSide;
	frontNortheastSubCell->centerY        = cellCenterY + cellQuarterSide;
	frontNortheastSubCell->centerZ        = cellCenterZ + cellQuarterSide;
	frontNortheastSubCell->bunchNumber    = oldBunchNumber * 10 + 1;

	// Creation of back Northeast node.
	backNortheastSubCell                 = new CellNode;
	backNortheastSubCell->level          = frontNortheastSubCell->level;
	backNortheastSubCell->active         = frontNortheastSubCell->active;
	backNortheastSubCell->faceLength     = cellHalfSide;
	backNortheastSubCell->halfFaceLength = cellQuarterSide;
	backNortheastSubCell->centerX        = cellCenterX - cellQuarterSide;
	backNortheastSubCell->centerY        = cellCenterY + cellQuarterSide;
	backNortheastSubCell->centerZ        = cellCenterZ + cellQuarterSide;
	backNortheastSubCell->v              = frontNortheastSubCell->v;
	for(int i = 0; i < NUMBER_EQUATIONS_CELL_MODEL; i++)
		backNortheastSubCell->sv[i] = frontNortheastSubCell->sv[i];
	backNortheastSubCell->previousU      = frontNortheastSubCell->previousU;
	backNortheastSubCell->bunchNumber    = oldBunchNumber * 10 + 2;

	// Creation of back Northwest node.
	backNorthwestSubCell                 = new CellNode;
	backNorthwestSubCell->level          = frontNortheastSubCell->level;
	backNorthwestSubCell->active         = frontNortheastSubCell->active;
	backNorthwestSubCell->faceLength     = cellHalfSide;
	backNorthwestSubCell->halfFaceLength = cellQuarterSide;
	backNorthwestSubCell->centerX        = cellCenterX - cellQuarterSide;
	backNorthwestSubCell->centerY        = cellCenterY - cellQuarterSide;
	backNorthwestSubCell->centerZ        = cellCenterZ + cellQuarterSide;
	backNorthwestSubCell->v              = frontNortheastSubCell->v;
	for(int i = 0; i < NUMBER_EQUATIONS_CELL_MODEL; i++)
		backNorthwestSubCell->sv[i] = frontNortheastSubCell->sv[i];
	backNorthwestSubCell->previousU      = frontNortheastSubCell->previousU;
	backNorthwestSubCell->bunchNumber    = oldBunchNumber * 10 + 3;

	// Creation of front Northwest node.
	frontNorthwestSubCell                 = new CellNode;
	frontNorthwestSubCell->level          = frontNortheastSubCell->level;
	frontNorthwestSubCell->active         = frontNortheastSubCell->active;
	frontNorthwestSubCell->faceLength     = cellHalfSide;
	frontNorthwestSubCell->halfFaceLength = cellQuarterSide;
	frontNorthwestSubCell->centerX        = cellCenterX + cellQuarterSide;
	frontNorthwestSubCell->centerY        = cellCenterY - cellQuarterSide;
	frontNorthwestSubCell->centerZ        = cellCenterZ + cellQuarterSide;
	frontNorthwestSubCell->v              = frontNortheastSubCell->v;
	for(int i = 0; i < NUMBER_EQUATIONS_CELL_MODEL; i++)
		frontNorthwestSubCell->sv[i] = frontNortheastSubCell->sv[i];
	frontNorthwestSubCell->previousU      = frontNortheastSubCell->previousU;
	frontNorthwestSubCell->bunchNumber    = oldBunchNumber * 10 + 4;

	// Creation of front Southwest node.
	frontSouthwestSubCell                 = new CellNode;
	frontSouthwestSubCell->level          = frontNortheastSubCell->level;
	frontSouthwestSubCell->active         = frontNortheastSubCell->active;
	frontSouthwestSubCell->faceLength     = cellHalfSide;
	frontSouthwestSubCell->halfFaceLength = cellQuarterSide;
	frontSouthwestSubCell->centerX        = cellCenterX + cellQuarterSide;
	frontSouthwestSubCell->centerY        = cellCenterY - cellQuarterSide;
	frontSouthwestSubCell->centerZ        = cellCenterZ - cellQuarterSide;
	frontSouthwestSubCell->v              = frontNortheastSubCell->v;
	for(int i = 0; i < NUMBER_EQUATIONS_CELL_MODEL; i++)
		frontSouthwestSubCell->sv[i] = frontNortheastSubCell->sv[i];
	frontSouthwestSubCell->previousU      = frontNortheastSubCell->previousU;
	frontSouthwestSubCell->bunchNumber    = oldBunchNumber * 10 + 5;

	// Creation of back Southwest node.
	backSouthwestSubCell                 = new CellNode;
	backSouthwestSubCell->level          = frontNortheastSubCell->level;
	backSouthwestSubCell->active         = frontNortheastSubCell->active;
	backSouthwestSubCell->faceLength     = cellHalfSide;
	backSouthwestSubCell->halfFaceLength = cellQuarterSide;
	backSouthwestSubCell->centerX        = cellCenterX - cellQuarterSide;
	backSouthwestSubCell->centerY        = cellCenterY - cellQuarterSide;
	backSouthwestSubCell->centerZ        = cellCenterZ - cellQuarterSide;
	backSouthwestSubCell->v              = frontNortheastSubCell->v;
	for(int i = 0; i < NUMBER_EQUATIONS_CELL_MODEL; i++)
		backSouthwestSubCell->sv[i] = frontNortheastSubCell->sv[i];
	backSouthwestSubCell->previousU      = frontNortheastSubCell->previousU;
	backSouthwestSubCell->bunchNumber    = oldBunchNumber * 10 + 6;

	// Creation of back Southeast node.
	backSoutheastSubCell                 = new CellNode;
	backSoutheastSubCell->level          = frontNortheastSubCell->level;
	backSoutheastSubCell->active         = frontNortheastSubCell->active;
	backSoutheastSubCell->faceLength     = cellHalfSide;
	backSoutheastSubCell->halfFaceLength = cellQuarterSide;
	backSoutheastSubCell->centerX        = cellCenterX - cellQuarterSide;
	backSoutheastSubCell->centerY        = cellCenterY + cellQuarterSide;
	backSoutheastSubCell->centerZ        = cellCenterZ - cellQuarterSide;
	backSoutheastSubCell->v              = frontNortheastSubCell->v;
	for(int i = 0; i < NUMBER_EQUATIONS_CELL_MODEL; i++)
		backSoutheastSubCell->sv[i] = frontNortheastSubCell->sv[i];
	backSoutheastSubCell->previousU      = frontNortheastSubCell->previousU;
	backSoutheastSubCell->bunchNumber    = oldBunchNumber * 10 + 7;

	// Creation of front Southeast node.
	frontSoutheastSubCell                 = new CellNode;
	frontSoutheastSubCell->level          = frontNortheastSubCell->level;
	frontSoutheastSubCell->active         = frontNortheastSubCell->active;
	frontSoutheastSubCell->faceLength     = cellHalfSide;
	frontSoutheastSubCell->halfFaceLength = cellQuarterSide;
	frontSoutheastSubCell->centerX        = cellCenterX + cellQuarterSide;
	frontSoutheastSubCell->centerY        = cellCenterY + cellQuarterSide;
	frontSoutheastSubCell->centerZ        = cellCenterZ - cellQuarterSide;
	frontSoutheastSubCell->v              = frontNortheastSubCell->v;
	for(int i = 0; i < NUMBER_EQUATIONS_CELL_MODEL; i++)
		frontSoutheastSubCell->sv[i] = frontNortheastSubCell->sv[i];
	frontSoutheastSubCell->previousU      = frontNortheastSubCell->previousU;
	frontSoutheastSubCell->bunchNumber    = oldBunchNumber * 10 + 8;

	// west transition node.
	westTransitionNode                  = new TransitionNode;
	westTransitionNode->direction       = 'w';
	westTransitionNode->level           = frontNortheastSubCell->level;
	westTransitionNode->active          = frontNortheastSubCell->active;
	westTransitionNode->centerX         = cellCenterX;
	westTransitionNode->centerY         = cellCenterY - cellHalfSide;
	westTransitionNode->centerZ         = cellCenterZ;
	westTransitionNode->singleConnector = frontNortheastSubCell->west;

	// north transition node.
	northTransitionNode                  = new TransitionNode;
	northTransitionNode->direction       = 'n';
	northTransitionNode->level           = frontNortheastSubCell->level;
	northTransitionNode->active          = frontNortheastSubCell->active;
	northTransitionNode->centerX         = cellCenterX;
	northTransitionNode->centerY         = cellCenterY;
	northTransitionNode->centerZ         = cellCenterZ + cellHalfSide;
	northTransitionNode->singleConnector = frontNortheastSubCell->north;

	// south transition node.
	southTransitionNode                  = new TransitionNode;
	southTransitionNode->direction       = 's';
	southTransitionNode->level           = frontNortheastSubCell->level;
	southTransitionNode->active          = frontNortheastSubCell->active;
	southTransitionNode->centerX         = cellCenterX;
	southTransitionNode->centerY         = cellCenterY;
	southTransitionNode->centerZ         = cellCenterZ - cellHalfSide;
	southTransitionNode->singleConnector = frontNortheastSubCell->south;

	// east transition node.
	eastTransitionNode                  = new TransitionNode;
	eastTransitionNode->direction       = 'e';
	eastTransitionNode->level           = frontNortheastSubCell->level;
	eastTransitionNode->active          = frontNortheastSubCell->active;
	eastTransitionNode->centerX         = cellCenterX;
	eastTransitionNode->centerY         = cellCenterY + cellHalfSide;
	eastTransitionNode->centerZ         = cellCenterZ;
	eastTransitionNode->singleConnector = frontNortheastSubCell->east;

	// front white node.
	frontTransitionNode                  = new TransitionNode;
	frontTransitionNode->direction       = 'f';
	frontTransitionNode->level           = frontNortheastSubCell->level;
	frontTransitionNode->active          = frontNortheastSubCell->active;
	frontTransitionNode->centerX         = cellCenterX + cellHalfSide;
	frontTransitionNode->centerY         = cellCenterY;
	frontTransitionNode->centerZ         = cellCenterZ;
	frontTransitionNode->singleConnector = frontNortheastSubCell->front;

	// back transitionition node.
	backTransitionNode                  = new TransitionNode;
	backTransitionNode->direction       = 'b';
	backTransitionNode->level           = frontNortheastSubCell->level;
	backTransitionNode->active          = frontNortheastSubCell->active;
	backTransitionNode->centerX         = cellCenterX - cellHalfSide;
	backTransitionNode->centerY         = cellCenterY;
	backTransitionNode->centerZ         = cellCenterZ;
	backTransitionNode->singleConnector = frontNortheastSubCell->back;

	// Linking of new cell nodes and transition nodes.
	frontNortheastSubCell->north = northTransitionNode;
	frontNortheastSubCell->south = frontSoutheastSubCell;
	frontNortheastSubCell->east  = eastTransitionNode;
	frontNortheastSubCell->west  = frontNorthwestSubCell;
	frontNortheastSubCell->front = frontTransitionNode;
	frontNortheastSubCell->back  = backNortheastSubCell;

	backNortheastSubCell->north = northTransitionNode;
	backNortheastSubCell->south = backSoutheastSubCell;
	backNortheastSubCell->east  = eastTransitionNode;
	backNortheastSubCell->west  = backNorthwestSubCell;
	backNortheastSubCell->front = frontNortheastSubCell;
	backNortheastSubCell->back  = backTransitionNode;

	backNorthwestSubCell->north = northTransitionNode;
	backNorthwestSubCell->south = backSouthwestSubCell;
	backNorthwestSubCell->east  = backNortheastSubCell;
	backNorthwestSubCell->west  = westTransitionNode;
	backNorthwestSubCell->front = frontNorthwestSubCell;
	backNorthwestSubCell->back  = backTransitionNode;

	frontNorthwestSubCell->north = northTransitionNode;
	frontNorthwestSubCell->south = frontSouthwestSubCell;
	frontNorthwestSubCell->east  = frontNortheastSubCell;
	frontNorthwestSubCell->west  = westTransitionNode;
	frontNorthwestSubCell->front = frontTransitionNode;
	frontNorthwestSubCell->back  = backNorthwestSubCell;

	frontSouthwestSubCell->north = frontNorthwestSubCell;
	frontSouthwestSubCell->south = southTransitionNode;
	frontSouthwestSubCell->east  = frontSoutheastSubCell;
	frontSouthwestSubCell->west  = westTransitionNode;
	frontSouthwestSubCell->front = frontTransitionNode;
	frontSouthwestSubCell->back  = backSouthwestSubCell;

	backSouthwestSubCell->north = backNorthwestSubCell;
	backSouthwestSubCell->south = southTransitionNode;
	backSouthwestSubCell->east  = backSoutheastSubCell;
	backSouthwestSubCell->west  = westTransitionNode;
	backSouthwestSubCell->front = frontSouthwestSubCell;
	backSouthwestSubCell->back  = backTransitionNode;

	backSoutheastSubCell->north = backNortheastSubCell;
	backSoutheastSubCell->south = southTransitionNode;
	backSoutheastSubCell->east  = eastTransitionNode;
	backSoutheastSubCell->west  = backSouthwestSubCell;
	backSoutheastSubCell->front = frontSoutheastSubCell;
	backSoutheastSubCell->back  = backTransitionNode;

	frontSoutheastSubCell->north = frontNortheastSubCell;
	frontSoutheastSubCell->south = southTransitionNode;
	frontSoutheastSubCell->east  = eastTransitionNode;
	frontSoutheastSubCell->west  = frontSouthwestSubCell;
	frontSoutheastSubCell->front = frontTransitionNode;
	frontSoutheastSubCell->back  = backSoutheastSubCell;

	/* Connects the cell nodes with the transition nodes.  Quadruple  connectors
	1, 2, 3 and 4 are connected to neighbor cells  in  the  way  depicted below.

	This choice is made consistent with the one made at the function simplifyRef(),
	so that when two transition nodes of  same  level  connected  through  their
	single connectors are eliminated, the  subsequent  linking  of  corresponding
	quadruple connectors is correctly done.

	                front face           back face

	                   ______              4______3
	                 /|     /|            /|     /|
	               4/_|___3/ |           /_|____/ |
	               |  |___|__|          |  |___|__|
	               | /    | /           | /1   | /2
	               |/_____|/            |/_____|/
	               1      2
	        ===========================================
	                west face           east face

	                  3______               ______3
	                 /|     /|            /|     /|
	               4/_|__ _/ |           /_|___4/ |
	               |  |___|__|          |  |___|__|
	               | /2   | /           | /    | /2
	               |/_____|/            |/_____|/
	               1                           1
	        ===========================================
	                north face             south face

	                  4______3              ______
	                 /|     /|            /|     /|
	               1/_|___2/ |           /_|____/ |
	               |  |___|__|          |  |___|__|
	               | /    | /           | /4   | /3
	               |/_____|/            |/_____|/
	                                    1      2
	        ===========================================
	 */

	// Front face.
	frontTransitionNode->quadrupleConnector1 = frontSouthwestSubCell;
	frontTransitionNode->quadrupleConnector2 = frontSoutheastSubCell;
	frontTransitionNode->quadrupleConnector3 = frontNortheastSubCell;
	frontTransitionNode->quadrupleConnector4 = frontNorthwestSubCell;

	// Back face.
	backTransitionNode->quadrupleConnector1 = backSouthwestSubCell;
	backTransitionNode->quadrupleConnector2 = backSoutheastSubCell;
	backTransitionNode->quadrupleConnector3 = backNortheastSubCell;
	backTransitionNode->quadrupleConnector4 = backNorthwestSubCell;

	// West face.
	westTransitionNode->quadrupleConnector1 = frontSouthwestSubCell;
	westTransitionNode->quadrupleConnector2 = backSouthwestSubCell;
	westTransitionNode->quadrupleConnector3 = backNorthwestSubCell;
	westTransitionNode->quadrupleConnector4 = frontNorthwestSubCell;

	// East face.
	eastTransitionNode->quadrupleConnector1 = frontSoutheastSubCell;
	eastTransitionNode->quadrupleConnector2 = backSoutheastSubCell;
	eastTransitionNode->quadrupleConnector3 = backNortheastSubCell;
	eastTransitionNode->quadrupleConnector4 = frontNortheastSubCell;

	// North face.
	northTransitionNode->quadrupleConnector1 = frontNorthwestSubCell;
	northTransitionNode->quadrupleConnector2 = frontNortheastSubCell;
	northTransitionNode->quadrupleConnector3 = backNortheastSubCell;
	northTransitionNode->quadrupleConnector4 = backNorthwestSubCell;

	// South face.
	southTransitionNode->quadrupleConnector1 = frontSouthwestSubCell;
	southTransitionNode->quadrupleConnector2 = frontSoutheastSubCell;
	southTransitionNode->quadrupleConnector3 = backSoutheastSubCell;
	southTransitionNode->quadrupleConnector4 = backSouthwestSubCell;



	// Linking bunch neighbor cells to the transition nodes just created.
	CellNode *neighborCellNode = NULL;
	TransitionNode *neighborTransitionNode = NULL;


	/*==========================================================================
	                            WEST TRANSITION NODE

	  Points the west neighboring cell to the transition node.

	                                     t  t               B: Bunch
	                                     | /                n: neighboring cell
	                           n -- t -- B -- t             w: Transition node
	                                   / |
	                                  t  t

	 *=========================================================================*/
	if( westTransitionNode->singleConnector->type == 'b' ) {
		neighborCellNode = static_cast<CellNode*>(westTransitionNode->singleConnector);
		neighborCellNode->east = westTransitionNode;
	}
	else if( westTransitionNode->singleConnector->type == 'w' ) {
		neighborTransitionNode = static_cast<TransitionNode*>(westTransitionNode->singleConnector);

		if( neighborTransitionNode->singleConnector == frontNortheastSubCell )
			neighborTransitionNode->singleConnector = westTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector1 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector1 = westTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector2 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector2 = westTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector3 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector3 = westTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector4 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector4 = westTransitionNode;
	}

	/*==========================================================================
	                         NORTH TRANSITION NODE

	  Points the north neighboring cell to the transition node.

		                              n
		                              |                  B: Bunch
		                              t  t               n: neighboring cell
		                              | /                t: Transition node
		                         t -- B -- t
		                            / |
		                           t  t

	==========================================================================*/
	if( northTransitionNode->singleConnector->type == 'b' ) {
		neighborCellNode = static_cast<CellNode*>(northTransitionNode->singleConnector);
		neighborCellNode->south = northTransitionNode;
	}
	else if( northTransitionNode->singleConnector->type == 'w' )	{
		neighborTransitionNode = static_cast<TransitionNode*>(northTransitionNode->singleConnector);

		if( neighborTransitionNode->singleConnector == frontNortheastSubCell )
			neighborTransitionNode->singleConnector = northTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector1 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector1 = northTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector2 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector2 = northTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector3 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector3 = northTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector4 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector4 = northTransitionNode;
	}

	/*==========================================================================
	                            SOUTH TRANSITION NODE

	  Points the south neighboring cell to the transition node.

		                              t  t               B: Bunch
		                              | /                n: neighboring cell
		                         t -- B -- t             w: Transition node
		                            / |
		                           t  t
		                              |
		                              n

	==========================================================================*/
	if( southTransitionNode->singleConnector->type == 'b' ) {
		neighborCellNode = static_cast<CellNode*>(southTransitionNode->singleConnector);
		neighborCellNode->north = southTransitionNode;
	}
	else if( southTransitionNode->singleConnector->type == 'w' )	{
		neighborTransitionNode = static_cast<TransitionNode*>(southTransitionNode->singleConnector);

		if( neighborTransitionNode->singleConnector == frontNortheastSubCell )
			neighborTransitionNode->singleConnector = southTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector1 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector1 = southTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector2 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector2 = southTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector3 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector3 = southTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector4 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector4 = southTransitionNode;
	}

	/*==========================================================================
	                          EAST TRANSITION NODE

		Points the east neighboring cell to the transition node.

		                             t  t               B: Bunch
		                             | /                n: neighboring cell
		                        t -- B -- t -- n        w: Transition node
		                           / |
		                          t  t

	==========================================================================*/
	if( eastTransitionNode->singleConnector->type == 'b' ) {
		neighborCellNode = static_cast<CellNode*>(eastTransitionNode->singleConnector);
		neighborCellNode->west = eastTransitionNode;
	}
	else if( eastTransitionNode->singleConnector->type == 'w' ) {
		neighborTransitionNode = static_cast<TransitionNode*>(eastTransitionNode->singleConnector);

		if( neighborTransitionNode->singleConnector == frontNortheastSubCell )
			neighborTransitionNode->singleConnector = eastTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector1 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector1 = eastTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector2 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector2 = eastTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector3 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector3 = eastTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector4 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector4 = eastTransitionNode;
	}

	/*==========================================================================
	                            FRONT TRANSITION NODE

	  Points the east neighboring cell to the transition node.

		                             t  t               B: Bunch
		                             | /                n: neighboring cell
		                        t -- B -- t             t: TRANSITION node
		                           / |
		                          t  t
		                         /
		                        n

	==========================================================================*/
	if( frontTransitionNode->singleConnector->type == 'b' ) {
		neighborCellNode = static_cast<CellNode*>(frontTransitionNode->singleConnector);
		neighborCellNode->back = frontTransitionNode;
	}
	else if( frontTransitionNode->singleConnector->type == 'w' ) {
		neighborTransitionNode = static_cast<TransitionNode*>(frontTransitionNode->singleConnector);

		if( neighborTransitionNode->singleConnector == frontNortheastSubCell )
			neighborTransitionNode->singleConnector = frontTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector1 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector1 = frontTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector2 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector2 = frontTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector3 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector3 = frontTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector4 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector4 = frontTransitionNode;
	}

	/*==========================================================================
	                          BACK TRANSITION NODE

	  Points the east neighboring cell to the transition node.

		                                  n             B: Bunch
		                                 /              n: neighboring cell
		                             t  t               t: Transition Node node
		                             | /
		                        t -- B -- t
		                           / |
		                          t  t

	==========================================================================*/
	if( backTransitionNode->singleConnector->type == 'b' ) {
		neighborCellNode = static_cast<CellNode*>(backTransitionNode->singleConnector);
		neighborCellNode->front = backTransitionNode;
	}
	else if( backTransitionNode->singleConnector->type == 'w' ) {
		neighborTransitionNode = static_cast<TransitionNode*>(backTransitionNode->singleConnector);

		if( neighborTransitionNode->singleConnector == frontNortheastSubCell )
			neighborTransitionNode->singleConnector = backTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector1 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector1 = backTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector2 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector2 = backTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector3 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector3 = backTransitionNode;

		else if( neighborTransitionNode->quadrupleConnector4 == frontNortheastSubCell )
			neighborTransitionNode->quadrupleConnector4 = backTransitionNode;
	}


	/*==========================================================================
	            ORDERING OF CELL NODES THROUGH HILBERT'S CURVE
	==========================================================================*/
	numberOfHilbertShape = frontNortheastSubCell->hilbertShapeNumber;

	if( numberOfHilbertShape == 0 )	{
		/* Shape 0
		                    _______
		                   /      /      b: begin
		                  /     b/       e: end
		                  |  ______
		                  | /     /
		                  |/    e/
		 */

		frontNortheastSubCell->hilbertShapeNumber = 1;
		backNortheastSubCell->hilbertShapeNumber  = 2;
		backNorthwestSubCell->hilbertShapeNumber  = 2;
		frontNorthwestSubCell->hilbertShapeNumber = 3;
		frontSouthwestSubCell->hilbertShapeNumber = 3;
		backSouthwestSubCell->hilbertShapeNumber  = 4;
		backSoutheastSubCell->hilbertShapeNumber  = 4;
		frontSoutheastSubCell->hilbertShapeNumber = 5;

		frontSoutheastSubCell->next = frontNortheastSubCell->next;
		frontNortheastSubCell->next = backNortheastSubCell;
		backNortheastSubCell->next  = backNorthwestSubCell;
		backNorthwestSubCell->next  = frontNorthwestSubCell;
		frontNorthwestSubCell->next = frontSouthwestSubCell;
		frontSouthwestSubCell->next = backSouthwestSubCell;
		backSouthwestSubCell->next  = backSoutheastSubCell;
		backSoutheastSubCell->next  = frontSoutheastSubCell;

		frontSoutheastSubCell->previous = backSoutheastSubCell;
		backSoutheastSubCell->previous  = backSouthwestSubCell;
		backSouthwestSubCell->previous  = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = backNorthwestSubCell;
		backNorthwestSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = frontNortheastSubCell;

		if( frontSoutheastSubCell->next != 0 )
			frontSoutheastSubCell->next->previous = frontSoutheastSubCell;

	}

	else if( numberOfHilbertShape == 1 ) {
		/* Shape 1
		                               e
		                       /|      |      b: begin
		                     /  |   b  |      e: end
		                     |  |___|__|
		                     |      |
		                     |______|
		 */

		frontNortheastSubCell->hilbertShapeNumber = 0;
		frontSoutheastSubCell->hilbertShapeNumber = 2;
		frontSouthwestSubCell->hilbertShapeNumber = 2;
		frontNorthwestSubCell->hilbertShapeNumber = 6;
		backNorthwestSubCell->hilbertShapeNumber  = 6;
		backSouthwestSubCell->hilbertShapeNumber  = 7;
		backSoutheastSubCell->hilbertShapeNumber  = 7;
		backNortheastSubCell->hilbertShapeNumber  = 8;

		backNortheastSubCell->next  = frontNortheastSubCell->next;
		frontNortheastSubCell->next = frontSoutheastSubCell;
		frontSoutheastSubCell->next = frontSouthwestSubCell;
		frontSouthwestSubCell->next = frontNorthwestSubCell;
		frontNorthwestSubCell->next = backNorthwestSubCell;
		backNorthwestSubCell->next  = backSouthwestSubCell;
		backSouthwestSubCell->next  = backSoutheastSubCell;
		backSoutheastSubCell->next  = backNortheastSubCell;

		backNortheastSubCell->previous  = backSoutheastSubCell;
		backSoutheastSubCell->previous  = backSouthwestSubCell;
		backSouthwestSubCell->previous  = backNorthwestSubCell;
		backNorthwestSubCell->previous  = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = frontNortheastSubCell;

		if( backNortheastSubCell->next != 0 )
			backNortheastSubCell->next->previous = backNortheastSubCell;

	}

	else if( numberOfHilbertShape == 2 ) {
		/* Shape 2
		                       /|     /|      b: begin
		                     e/ |   b/ |      e: end
		                        |      |
		                       /      /
		                      /______/
		 */

		frontNortheastSubCell->hilbertShapeNumber = 1;
		backNortheastSubCell->hilbertShapeNumber  = 0;
		backSoutheastSubCell->hilbertShapeNumber  = 0;
		frontSoutheastSubCell->hilbertShapeNumber = 9;
		frontSouthwestSubCell->hilbertShapeNumber = 9;
		backSouthwestSubCell->hilbertShapeNumber  = 10;
		backNorthwestSubCell->hilbertShapeNumber  = 10;
		frontNorthwestSubCell->hilbertShapeNumber = 11;

		frontNorthwestSubCell->next = frontNortheastSubCell->next;
		frontNortheastSubCell->next = backNortheastSubCell;
		backNortheastSubCell->next  = backSoutheastSubCell;
		backSoutheastSubCell->next  = frontSoutheastSubCell;
		frontSoutheastSubCell->next = frontSouthwestSubCell;
		frontSouthwestSubCell->next = backSouthwestSubCell;
		backSouthwestSubCell->next  = backNorthwestSubCell;
		backNorthwestSubCell->next  = frontNorthwestSubCell;

		frontNorthwestSubCell->previous = backNorthwestSubCell;
		backNorthwestSubCell->previous  = backSouthwestSubCell;
		backSouthwestSubCell->previous  = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = backSoutheastSubCell;
		backSoutheastSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = frontNortheastSubCell;

		if( frontNorthwestSubCell->next != 0 )
			frontNorthwestSubCell->next->previous = frontNorthwestSubCell;

	}

	else if( numberOfHilbertShape == 3 ) {
		/* Shape 3
		                       /b     /|      b: begin
		                      /______/ |      e: end
		                               |
		                       /e     /
		                      /______/
		 */

		backNorthwestSubCell->hilbertShapeNumber  = 11;
		frontNorthwestSubCell->hilbertShapeNumber = 7;
		frontNortheastSubCell->hilbertShapeNumber = 7;
		backNortheastSubCell->hilbertShapeNumber  = 0;
		backSoutheastSubCell->hilbertShapeNumber  = 0;
		frontSoutheastSubCell->hilbertShapeNumber = 9;
		frontSouthwestSubCell->hilbertShapeNumber = 9;
		backSouthwestSubCell->hilbertShapeNumber  = 6;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = backNorthwestSubCell;

		backSouthwestSubCell->next  = frontNortheastSubCell->next;
		backNorthwestSubCell->next  = frontNorthwestSubCell;
		frontNorthwestSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = backNortheastSubCell;
		backNortheastSubCell->next  = backSoutheastSubCell;
		backSoutheastSubCell->next  = frontSoutheastSubCell;
		frontSoutheastSubCell->next = frontSouthwestSubCell;
		frontSouthwestSubCell->next = backSouthwestSubCell;

		backNorthwestSubCell->previous  = frontNortheastSubCell->previous;
		backSouthwestSubCell->previous  = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = backSoutheastSubCell;
		backSoutheastSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = backNorthwestSubCell;

		if( backSouthwestSubCell->next != 0 )
			backSouthwestSubCell->next->previous = backSouthwestSubCell;

	}

	else if ( numberOfHilbertShape == 4 ) {
		/* Shape 4
			                    /|     /|      b: begin
			                   /_|____/ |      e: end
			                     |      |
			                    /      /
			                  b/     e/
		 */

		frontSouthwestSubCell->hilbertShapeNumber = 6;
		backSouthwestSubCell->hilbertShapeNumber  = 10;
		backNorthwestSubCell->hilbertShapeNumber  = 10;
		frontNorthwestSubCell->hilbertShapeNumber = 7;
		frontNortheastSubCell->hilbertShapeNumber = 7;
		backNortheastSubCell->hilbertShapeNumber  = 0;
		backSoutheastSubCell->hilbertShapeNumber  = 0;
		frontSoutheastSubCell->hilbertShapeNumber = 5;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = frontSouthwestSubCell;

		frontSoutheastSubCell->next = frontNortheastSubCell->next;
		frontSouthwestSubCell->next = backSouthwestSubCell;
		backSouthwestSubCell->next  = backNorthwestSubCell;
		backNorthwestSubCell->next  = frontNorthwestSubCell;
		frontNorthwestSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = backNortheastSubCell;
		backNortheastSubCell->next  = backSoutheastSubCell;
		backSoutheastSubCell->next  = frontSoutheastSubCell;

		frontSouthwestSubCell->previous = frontNortheastSubCell->previous;
		frontSoutheastSubCell->previous = backSoutheastSubCell;
		backSoutheastSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = backNorthwestSubCell;
		backNorthwestSubCell->previous  = backSouthwestSubCell;
		backSouthwestSubCell->previous  = frontSouthwestSubCell;

		if ( frontSoutheastSubCell->next != 0 )
			frontSoutheastSubCell->next->previous = frontSoutheastSubCell;

	}

	else if( numberOfHilbertShape == 5 ) {
		/* Shape 5
			                     ______
			                    |      |      b: begin
			                  __|___   |      e: end
			                 |  |   |  |b
			                 | /    |
			                 |/     |e
		 */

		backSoutheastSubCell->hilbertShapeNumber  = 8;
		backNortheastSubCell->hilbertShapeNumber  = 9;
		backNorthwestSubCell->hilbertShapeNumber  = 9;
		backSouthwestSubCell->hilbertShapeNumber  = 11;
		frontSouthwestSubCell->hilbertShapeNumber = 11;
		frontNorthwestSubCell->hilbertShapeNumber = 4;
		frontNortheastSubCell->hilbertShapeNumber = 4;
		frontSoutheastSubCell->hilbertShapeNumber = 0;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = backSoutheastSubCell;

		frontSoutheastSubCell->next = frontNortheastSubCell->next;
		backSoutheastSubCell->next  = backNortheastSubCell;
		backNortheastSubCell->next  = backNorthwestSubCell;
		backNorthwestSubCell->next  = backSouthwestSubCell;
		backSouthwestSubCell->next  = frontSouthwestSubCell;
		frontSouthwestSubCell->next = frontNorthwestSubCell;
		frontNorthwestSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = frontSoutheastSubCell;

		backSoutheastSubCell->previous  = frontNortheastSubCell->previous;
		frontSoutheastSubCell->previous = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = backSouthwestSubCell;
		backSouthwestSubCell->previous  = backNorthwestSubCell;
		backNorthwestSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = backSoutheastSubCell;

		if( frontSoutheastSubCell->next != 0 )
			frontSoutheastSubCell->next->previous = frontSoutheastSubCell;

	}

	else if( numberOfHilbertShape == 6 ) {
		/* Shape 6
			                     ______
			                    |      |      b: begin
			                  __|___   |      e: end
			                 | e|   |  |
			                 |      | /
			                b|      |/
		 */

		frontSouthwestSubCell->hilbertShapeNumber = 10;
		frontNorthwestSubCell->hilbertShapeNumber = 4;
		frontNortheastSubCell->hilbertShapeNumber = 4;
		frontSoutheastSubCell->hilbertShapeNumber = 1;
		backSoutheastSubCell->hilbertShapeNumber  = 1;
		backNortheastSubCell->hilbertShapeNumber  = 9;
		backNorthwestSubCell->hilbertShapeNumber  = 9;
		backSouthwestSubCell->hilbertShapeNumber  = 3;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = frontSouthwestSubCell;

		backSouthwestSubCell->next  = frontNortheastSubCell->next;
		frontSouthwestSubCell->next = frontNorthwestSubCell;
		frontNorthwestSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = frontSoutheastSubCell;
		frontSoutheastSubCell->next = backSoutheastSubCell;
		backSoutheastSubCell->next  = backNortheastSubCell;
		backNortheastSubCell->next  = backNorthwestSubCell;
		backNorthwestSubCell->next  = backSouthwestSubCell;

		frontSouthwestSubCell->previous = frontNortheastSubCell->previous;
		backSouthwestSubCell->previous  = backNorthwestSubCell;
		backNorthwestSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = backSoutheastSubCell;
		backSoutheastSubCell->previous  = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = frontSouthwestSubCell;

		if( backSouthwestSubCell->next != 0 )
			backSouthwestSubCell->next->previous = backSouthwestSubCell;

	}

	else if( numberOfHilbertShape == 7 ) {
		/* Shape 7
			                    |b     |e     b: begin
			                  __|___   |      e: end
			                 |  |   |  |
			                 | /    | /
			                 |/     |/
		 */

		backNorthwestSubCell->hilbertShapeNumber  = 3;
		backSouthwestSubCell->hilbertShapeNumber  = 11;
		frontSouthwestSubCell->hilbertShapeNumber = 11;
		frontNorthwestSubCell->hilbertShapeNumber = 4;
		frontNortheastSubCell->hilbertShapeNumber = 4;
		frontSoutheastSubCell->hilbertShapeNumber = 1;
		backSoutheastSubCell->hilbertShapeNumber  = 1;
		backNortheastSubCell->hilbertShapeNumber  = 8;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = backNorthwestSubCell;

		backNortheastSubCell->next  = frontNortheastSubCell->next;
		backNorthwestSubCell->next  = backSouthwestSubCell;
		backSouthwestSubCell->next  = frontSouthwestSubCell;
		frontSouthwestSubCell->next = frontNorthwestSubCell;
		frontNorthwestSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = frontSoutheastSubCell;
		frontSoutheastSubCell->next = backSoutheastSubCell;
		backSoutheastSubCell->next  = backNortheastSubCell;

		backNorthwestSubCell->previous  = frontNortheastSubCell->previous;
		backNortheastSubCell->previous  = backSoutheastSubCell;
		backSoutheastSubCell->previous  = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = backSouthwestSubCell;
		backSouthwestSubCell->previous  = backNorthwestSubCell;

		if( backNortheastSubCell->next != 0 )
			backNortheastSubCell->next->previous = backNortheastSubCell;

	}

	else if( numberOfHilbertShape == 8 ) {
		/* Shape 8
			                   /|     /e      b: begin
			                  /_|____/        e: end
			                    |
			                   /      /b
			                  /______/
		 */

		backSoutheastSubCell->hilbertShapeNumber  = 5;
		frontSoutheastSubCell->hilbertShapeNumber = 9;
		frontSouthwestSubCell->hilbertShapeNumber = 9;
		backSouthwestSubCell->hilbertShapeNumber  = 10;
		backNorthwestSubCell->hilbertShapeNumber  = 10;
		frontNorthwestSubCell->hilbertShapeNumber = 7;
		frontNortheastSubCell->hilbertShapeNumber = 7;
		backNortheastSubCell->hilbertShapeNumber  = 1;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = backSoutheastSubCell;

		backNortheastSubCell->next  = frontNortheastSubCell->next;
		backSoutheastSubCell->next  = frontSoutheastSubCell;
		frontSoutheastSubCell->next = frontSouthwestSubCell;
		frontSouthwestSubCell->next = backSouthwestSubCell;
		backSouthwestSubCell->next  = backNorthwestSubCell;
		backNorthwestSubCell->next  = frontNorthwestSubCell;
		frontNorthwestSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = backNortheastSubCell;

		backSoutheastSubCell->previous  = frontNortheastSubCell->previous;
		backNortheastSubCell->previous  = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = backNorthwestSubCell;
		backNorthwestSubCell->previous  = backSouthwestSubCell;
		backSouthwestSubCell->previous  = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = backSoutheastSubCell;

		if( backNortheastSubCell->next != 0 )
			backNortheastSubCell->next->previous = backNortheastSubCell;

	}

	else if( numberOfHilbertShape == 9 ) {
		/* Shape 9
			                    _______
			                   /      /      b: begin
			                  /      /       e: end
			                  |      |
			                  | /e   | /b
			                  |/     |/
		 */

		backSoutheastSubCell->hilbertShapeNumber  = 5;
		frontSoutheastSubCell->hilbertShapeNumber = 8;
		frontNortheastSubCell->hilbertShapeNumber = 8;
		backNortheastSubCell->hilbertShapeNumber  = 2;
		backNorthwestSubCell->hilbertShapeNumber  = 2;
		frontNorthwestSubCell->hilbertShapeNumber = 3;
		frontSouthwestSubCell->hilbertShapeNumber = 3;
		backSouthwestSubCell->hilbertShapeNumber  = 6;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = backSoutheastSubCell;

		backSouthwestSubCell->next  = frontNortheastSubCell->next;
		backSoutheastSubCell->next  = frontSoutheastSubCell;
		frontSoutheastSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = backNortheastSubCell;
		backNortheastSubCell->next  = backNorthwestSubCell;
		backNorthwestSubCell->next  = frontNorthwestSubCell;
		frontNorthwestSubCell->next = frontSouthwestSubCell;
		frontSouthwestSubCell->next = backSouthwestSubCell;

		backSoutheastSubCell->previous  = frontNortheastSubCell->previous;
		backSouthwestSubCell->previous  = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = frontNorthwestSubCell;
		frontNorthwestSubCell->previous = backNorthwestSubCell;
		backNorthwestSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = backSoutheastSubCell;

		if( backSouthwestSubCell->next != 0 )
			backSouthwestSubCell->next->previous = backSouthwestSubCell;

	}

	else if( numberOfHilbertShape == 10 ) {
		/* Shape 10
		                         _______
		                        /      /      b: begin
		                      e/      /       e: end
		                          ____|__
		                         /    | /
		                       b/     |/
		 */

		frontSouthwestSubCell->hilbertShapeNumber = 6;
		backSouthwestSubCell->hilbertShapeNumber  = 4;
		backSoutheastSubCell->hilbertShapeNumber  = 4;
		frontSoutheastSubCell->hilbertShapeNumber = 8;
		frontNortheastSubCell->hilbertShapeNumber = 8;
		backNortheastSubCell->hilbertShapeNumber  = 2;
		backNorthwestSubCell->hilbertShapeNumber  = 2;
		frontNorthwestSubCell->hilbertShapeNumber = 11;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = frontSouthwestSubCell;

		frontNorthwestSubCell->next = frontNortheastSubCell->next;
		frontSouthwestSubCell->next = backSouthwestSubCell;
		backSouthwestSubCell->next  = backSoutheastSubCell;
		backSoutheastSubCell->next  = frontSoutheastSubCell;
		frontSoutheastSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = backNortheastSubCell;
		backNortheastSubCell->next  = backNorthwestSubCell;
		backNorthwestSubCell->next  = frontNorthwestSubCell;

		frontSouthwestSubCell->previous = frontNortheastSubCell->previous;
		frontNorthwestSubCell->previous = backNorthwestSubCell;
		backNorthwestSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = backSoutheastSubCell;
		backSoutheastSubCell->previous  = backSouthwestSubCell;
		backSouthwestSubCell->previous  = frontSouthwestSubCell;

		if( frontNorthwestSubCell->next != 0 )
			frontNorthwestSubCell->next->previous = frontNorthwestSubCell;

	}

	else if( numberOfHilbertShape == 11 ) {
		/* Shape 11
			                         b______
			                                |      b: begin
			                       e______  |      e: end
			                         _____|_|
			                        /     |
			                       /______|
		 */

		backNorthwestSubCell->hilbertShapeNumber  = 7;
		backNortheastSubCell->hilbertShapeNumber  = 3;
		backSoutheastSubCell->hilbertShapeNumber  = 3;
		backSouthwestSubCell->hilbertShapeNumber  = 5;
		frontSouthwestSubCell->hilbertShapeNumber = 5;
		frontSoutheastSubCell->hilbertShapeNumber = 10;
		frontNortheastSubCell->hilbertShapeNumber = 10;
		frontNorthwestSubCell->hilbertShapeNumber = 2;

		if( frontNortheastSubCell->previous != 0 )
			frontNortheastSubCell->previous->next = backNorthwestSubCell;

		frontNorthwestSubCell->next = frontNortheastSubCell->next;
		backNorthwestSubCell->next  = backNortheastSubCell;
		backNortheastSubCell->next  = backSoutheastSubCell;
		backSoutheastSubCell->next  = backSouthwestSubCell;
		backSouthwestSubCell->next  = frontSouthwestSubCell;
		frontSouthwestSubCell->next = frontSoutheastSubCell;
		frontSoutheastSubCell->next = frontNortheastSubCell;
		frontNortheastSubCell->next = frontNorthwestSubCell;

		backNorthwestSubCell->previous  = frontNortheastSubCell->previous;
		frontNorthwestSubCell->previous = frontNortheastSubCell;
		frontNortheastSubCell->previous = frontSoutheastSubCell;
		frontSoutheastSubCell->previous = frontSouthwestSubCell;
		frontSouthwestSubCell->previous = backSouthwestSubCell;
		backSouthwestSubCell->previous  = backSoutheastSubCell;
		backSoutheastSubCell->previous  = backNortheastSubCell;
		backNortheastSubCell->previous  = backNorthwestSubCell;

		if( frontNorthwestSubCell->next != 0 )
			frontNorthwestSubCell->next->previous = frontNorthwestSubCell;

	}

	// If necessary, simplifies the graph by eliminating adjacent transition nodes
	// of same level connected through their single connectors.
	simplifyRef( eastTransitionNode  );
	simplifyRef( northTransitionNode );
	simplifyRef( westTransitionNode  );
	simplifyRef( southTransitionNode );
	simplifyRef( frontTransitionNode );
	simplifyRef( backTransitionNode  );
}

/**
 * Simplifies data structure eliminating adjacent transition nodes of same level.
 *
 * @param transitionNode Candidate transition node to be eliminated.
 * @throw NullPointer If a null transition node is given as argument, a NullPointer
 * exception is thrown.
 */
void Grid::simplifyRef( TransitionNode *transitionNode ) throw (NullPointer) {
	if( transitionNode == 0 ) {
		throw NullPointer
		("Grid::simplifyGraph(): Parameter transitionNode is NULL.");
	}

	// Pointers used to convert the Cell in a cell node or transition node.
	TransitionNode *neighborTransitionNode;
	CellNode *neighborCellNode;

	if( transitionNode->singleConnector != 0 ) {

		// Both transition node and neighbor transition node must have the same
		// refinement level.
		if( ( transitionNode->type == 'w') && (transitionNode->level == transitionNode->singleConnector->level) ) {
			TransitionNode *neighborNode = static_cast<TransitionNode*> (transitionNode->singleConnector);

			CellNode *cellNode[4];
			cellNode[0] = static_cast<CellNode*>(transitionNode->quadrupleConnector1);
			cellNode[1] = static_cast<CellNode*>(transitionNode->quadrupleConnector2);
			cellNode[2] = static_cast<CellNode*>(transitionNode->quadrupleConnector3);
			cellNode[3] = static_cast<CellNode*>(transitionNode->quadrupleConnector4);

			Cell *neighborCell[4];
			neighborCell[0] = neighborNode->quadrupleConnector1;
			neighborCell[1] = neighborNode->quadrupleConnector2;
			neighborCell[2] = neighborNode->quadrupleConnector3;
			neighborCell[3] = neighborNode->quadrupleConnector4;

			char direction = transitionNode->direction;
			char type;

			for( int i = 0; i < 4; i++ ) {
				switch( direction ) {
				case 'n': { cellNode[i]->north = neighborCell[i]; break; }
				case 's': { cellNode[i]->south = neighborCell[i]; break; }
				case 'e': { cellNode[i]->east  = neighborCell[i]; break; }
				case 'w': { cellNode[i]->west  = neighborCell[i]; break; }
				case 'f': { cellNode[i]->front = neighborCell[i]; break; }
				case 'b': { cellNode[i]->back  = neighborCell[i]; break; }
				default: break;
				}

				type = neighborCell[i]->type;
				switch( type ) {
				case 'b': {
					neighborCellNode = static_cast<CellNode*> (neighborCell[i]);
					switch( direction )	{
					case 'n': { neighborCellNode->south = cellNode[i]; break; }
					case 's': { neighborCellNode->north = cellNode[i]; break; }
					case 'e': { neighborCellNode->west  = cellNode[i]; break; }
					case 'w': { neighborCellNode->east  = cellNode[i]; break; }
					case 'f': { neighborCellNode->back  = cellNode[i]; break; }
					case 'b': { neighborCellNode->front = cellNode[i]; break; }
					default: break;
					}
					break;
				}

				case 'w': {
					neighborTransitionNode = static_cast<TransitionNode*>(neighborCell[i]);
					if( neighborNode == neighborTransitionNode->singleConnector )
						neighborTransitionNode->singleConnector = cellNode[i];

					else if( neighborNode == neighborTransitionNode->quadrupleConnector1 )
						neighborTransitionNode->quadrupleConnector1 = cellNode[i];

					else if( neighborNode == neighborTransitionNode->quadrupleConnector2 )
						neighborTransitionNode->quadrupleConnector2 = cellNode[i];

					else if( neighborNode == neighborTransitionNode->quadrupleConnector3 )
						neighborTransitionNode->quadrupleConnector3 = cellNode[i];

					else if( neighborNode == neighborTransitionNode->quadrupleConnector4 )
						neighborTransitionNode->quadrupleConnector4 = cellNode[i];

					break;
				}

				default: break;
				}
			}
			delete transitionNode;
			delete neighborNode;
		}
	}
}
