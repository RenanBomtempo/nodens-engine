#include "../grid.h"

/*==============================================================================
                             FUNCTION refineCell()

    Refines a cell, tranforming it into a bunch with eigth cell nodes and six
    transition nodes. It receive a black node as parameter.

==============================================================================*/
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

	float cellCenterX     = cell->centerX,
		  cellCenterY     = cell->centerY,
		  cellCenterZ     = cell->centerZ,
		  cellHalfSide    = cell->halfFaceLength,
		  cellQuarterSide = cell->halfFaceLength / 2.0;

	long int oldBunchNumber = cell->bunchNumber;

	/*===========================================================================

	                     CREATION OF THE FRONTNORTHEAST CELL

	  This cell, which is to be refined, becomes the frontNortheast cell of the
	  new bunch.
	 =========================================================================*/
	frontNortheastSubCell                 = cell;
	frontNortheastSubCell->level          = cell->level + 1;
	frontNortheastSubCell->faceLength     = cellHalfSide;
	frontNortheastSubCell->halfFaceLength = cellQuarterSide;
	frontNortheastSubCell->centerX        = cellCenterX + cellQuarterSide;
	frontNortheastSubCell->centerY        = cellCenterY + cellQuarterSide;
	frontNortheastSubCell->centerZ        = cellCenterZ + cellQuarterSide;
	frontNortheastSubCell->bunchNumber    = oldBunchNumber * 10 + 1;

	/*==========================================================================
	                        CREATION OF 7 NEW BLACK NODES
	 ==========================================================================*/

	// Creation of back Northeast node.
	backNortheastSubCell                 = new CellNode;
	backNortheastSubCell->level          = frontNortheastSubCell->level;
	backNortheastSubCell->active         = frontNortheastSubCell->active;
	backNortheastSubCell->faceLength     = cellHalfSide;
	backNortheastSubCell->halfFaceLength = cellQuarterSide;
	backNortheastSubCell->centerX        = cellCenterX - cellQuarterSide;
	backNortheastSubCell->centerY        = cellCenterY + cellQuarterSide;
	backNortheastSubCell->centerZ        = cellCenterZ + cellQuarterSide;
	backNortheastSubCell->u              = frontNortheastSubCell->u;
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
	backNorthwestSubCell->u              = frontNortheastSubCell->u;
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
	frontNorthwestSubCell->u              = frontNortheastSubCell->u;
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
	frontSouthwestSubCell->u              = frontNortheastSubCell->u;
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
	backSouthwestSubCell->u              = frontNortheastSubCell->u;
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
	backSoutheastSubCell->u              = frontNortheastSubCell->u;
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
	frontSoutheastSubCell->u              = frontNortheastSubCell->u;
	frontSoutheastSubCell->previousU      = frontNortheastSubCell->previousU;
	frontSoutheastSubCell->bunchNumber    = oldBunchNumber * 10 + 8;

	/*==========================================================================
	                         CREATION OF WHITE NODES

	                 Creates 6 white nodes around the cell.
	 =========================================================================*/
	// west white node.
	westTransitionNode                  = new TransitionNode;
	westTransitionNode->direction       = 'w';
	westTransitionNode->level           = frontNortheastSubCell->level;
	westTransitionNode->active          = frontNortheastSubCell->active;
	westTransitionNode->centerX         = cellCenterX;
	westTransitionNode->centerY         = cellCenterY - cellHalfSide;
	westTransitionNode->centerZ         = cellCenterZ;
	westTransitionNode->singleConnector = frontNortheastSubCell->west;

	// north white node.
	northTransitionNode                  = new TransitionNode;
	northTransitionNode->direction       = 'n';
	northTransitionNode->level           = frontNortheastSubCell->level;
	northTransitionNode->active          = frontNortheastSubCell->active;
	northTransitionNode->centerX         = cellCenterX;
	northTransitionNode->centerY         = cellCenterY;
	northTransitionNode->centerZ         = cellCenterZ + cellHalfSide;
	northTransitionNode->singleConnector = frontNortheastSubCell->north;

	// south white node.
	southTransitionNode                  = new TransitionNode;
	southTransitionNode->direction       = 's';
	southTransitionNode->level           = frontNortheastSubCell->level;
	southTransitionNode->active          = frontNortheastSubCell->active;
	southTransitionNode->centerX         = cellCenterX;
	southTransitionNode->centerY         = cellCenterY;
	southTransitionNode->centerZ         = cellCenterZ - cellHalfSide;
	southTransitionNode->singleConnector = frontNortheastSubCell->south;

	// east white node.
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

	// back white node.
	backTransitionNode                  = new TransitionNode;
	backTransitionNode->direction       = 'b';
	backTransitionNode->level           = frontNortheastSubCell->level;
	backTransitionNode->active          = frontNortheastSubCell->active;
	backTransitionNode->centerX         = cellCenterX - cellHalfSide;
	backTransitionNode->centerY         = cellCenterY;
	backTransitionNode->centerZ         = cellCenterZ;
	backTransitionNode->singleConnector = frontNortheastSubCell->back;

	/*==========================================================================
	                 LINKING OF NEW CELL AND WHITE NODES


	                          b(NW) _____________ b(NE)       f: front
	                              /|            /|            b: back
	                            /  |          /  |
	                          /____|________/    |
	                     f(NW)|    |       |f(NE)|
	                          |    |       |     |
	                          |    |_______|_____|
	                          |   /b(SW)   |    / b(SE)
	                          | /          |  /
	                          |/___________|/
	                      f(SW)             f(SE)

	==========================================================================*/
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

	This choice is made consistent with the one made at the function simplifyGrid(),
	so that when two transition nodes of  same  level  connected  through  their
	single connectors are eliminated, the  subsequent  linking  of  corresponding
	quadruple connectors is correctly done.

	                Front face           Back face

	                   ______              4______3
	                 /|     /|            /|     /|
	               4/_|___3/ |           /_|____/ |
	               |  |___|__|          |  |___|__|
	               | /    | /           | /1   | /2
	               |/_____|/            |/_____|/
	               1      2
	        ===========================================
	                Left face           Right face

	                  3______               ______3
	                 /|     /|            /|     /|
	               4/_|__ _/ |           /_|___4/ |
	               |  |___|__|          |  |___|__|
	               | /2   | /           | /    | /2
	               |/_____|/            |/_____|/
	               1                           1
	        ===========================================
	                Top face             Down face

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

	// Left face.
	westTransitionNode->quadrupleConnector1 = frontSouthwestSubCell;
	westTransitionNode->quadrupleConnector2 = backSouthwestSubCell;
	westTransitionNode->quadrupleConnector3 = backNorthwestSubCell;
	westTransitionNode->quadrupleConnector4 = frontNorthwestSubCell;

	// Right face.
	eastTransitionNode->quadrupleConnector1 = frontSoutheastSubCell;
	eastTransitionNode->quadrupleConnector2 = backSoutheastSubCell;
	eastTransitionNode->quadrupleConnector3 = backNortheastSubCell;
	eastTransitionNode->quadrupleConnector4 = frontNortheastSubCell;

	// Top face.
	northTransitionNode->quadrupleConnector1 = frontNorthwestSubCell;
	northTransitionNode->quadrupleConnector2 = frontNortheastSubCell;
	northTransitionNode->quadrupleConnector3 = backNortheastSubCell;
	northTransitionNode->quadrupleConnector4 = backNorthwestSubCell;

	// Down face.
	southTransitionNode->quadrupleConnector1 = frontSouthwestSubCell;
	southTransitionNode->quadrupleConnector2 = frontSoutheastSubCell;
	southTransitionNode->quadrupleConnector3 = backSoutheastSubCell;
	southTransitionNode->quadrupleConnector4 = backSouthwestSubCell;


	//==========================================================================
	//       Linking bunch neighbor cells to the white nodes just created.
	//==========================================================================
	CellNode *blackNeighborCell = NULL;
	TransitionNode *whiteNeighborCell = NULL;


	/*==========================================================================
	                            WEST WHITE NODE

	  Points the west neighboring cell to the transition node.

	                                     w  w               B: Bunch
	                                     | /                n: neighboring cell
	                           n -- w -- B -- w             w: white node
	                                   / |
	                                  w  w

	*=========================================================================*/
	if( westTransitionNode->singleConnector->type == 'b' ) {
		blackNeighborCell = static_cast<CellNode*>(westTransitionNode->singleConnector);
		blackNeighborCell->east = westTransitionNode;
	}
	else if( westTransitionNode->singleConnector->type == 'w' ) {
			whiteNeighborCell = static_cast<TransitionNode*>(westTransitionNode->singleConnector);

			if( whiteNeighborCell->singleConnector == frontNortheastSubCell )
				whiteNeighborCell->singleConnector = westTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector1 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector1 = westTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector2 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector2 = westTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector3 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector3 = westTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector4 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector4 = westTransitionNode;
	}

	/*==========================================================================
	                         NORTH WHITE NODE

	  Points the north neighboring cell to the transition node.

		                              n
		                              |                  B: Bunch
		                              w  w               n: neighboring cell
		                              | /                w: white node
		                         w -- B -- w
		                            / |
		                           w  w

	==========================================================================*/
	if( northTransitionNode->singleConnector->type == 'b' ) {
		blackNeighborCell = static_cast<CellNode*>(northTransitionNode->singleConnector);
		blackNeighborCell->south = northTransitionNode;
	}
	else if( northTransitionNode->singleConnector->type == 'w' )	{
			whiteNeighborCell = static_cast<TransitionNode*>(northTransitionNode->singleConnector);

			if( whiteNeighborCell->singleConnector == frontNortheastSubCell )
				whiteNeighborCell->singleConnector = northTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector1 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector1 = northTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector2 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector2 = northTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector3 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector3 = northTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector4 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector4 = northTransitionNode;
	}

	/*==========================================================================
	                            SOUTH WHITE NODE

	  Points the south neighboring cell to the transition node.

		                              w  w               B: Bunch
		                              | /                n: neighboring cell
		                         w -- B -- w             w: white node
		                            / |
		                           w  w
		                              |
		                              n

	==========================================================================*/
	if( southTransitionNode->singleConnector->type == 'b' ) {
		blackNeighborCell = static_cast<CellNode*>(southTransitionNode->singleConnector);
		blackNeighborCell->north = southTransitionNode;
	}
	else if( southTransitionNode->singleConnector->type == 'w' )	{
			whiteNeighborCell = static_cast<TransitionNode*>(southTransitionNode->singleConnector);

			if( whiteNeighborCell->singleConnector == frontNortheastSubCell )
				whiteNeighborCell->singleConnector = southTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector1 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector1 = southTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector2 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector2 = southTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector3 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector3 = southTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector4 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector4 = southTransitionNode;
	}

	/*==========================================================================
	                          EAST WHITE NODE

		Points the east neighboring cell to the transition node.

		                             w  w               B: Bunch
		                             | /                n: neighboring cell
		                        w -- B -- w -- n        w: white node
		                           / |
		                          w  w

	==========================================================================*/
	if( eastTransitionNode->singleConnector->type == 'b' ) {
		blackNeighborCell = static_cast<CellNode*>(eastTransitionNode->singleConnector);
		blackNeighborCell->west = eastTransitionNode;
	}
	else if( eastTransitionNode->type == 'w' ) {
			whiteNeighborCell = static_cast<TransitionNode*>(eastTransitionNode->singleConnector);

			if( whiteNeighborCell->singleConnector == frontNortheastSubCell )
				whiteNeighborCell->singleConnector = eastTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector1 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector1 = eastTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector2 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector2 = eastTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector3 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector3 = eastTransitionNode;

			else if( whiteNeighborCell->quadrupleConnector4 == frontNortheastSubCell )
					whiteNeighborCell->quadrupleConnector4 = eastTransitionNode;
	}

	/*==========================================================================
	                            FRONT WHITE NODE

	  Points the east neighboring cell to the transition node.

		                             w  w               B: Bunch
		                             | /                n: neighboring cell
		                        w -- B -- w             w: white node
		                           / |
		                          w  w
		                         /
		                        n

	==========================================================================*/
	if( frontTransitionNode->singleConnector->type == 'b' ) {
		blackNeighborCell = static_cast<CellNode*>(frontTransitionNode->singleConnector);
		blackNeighborCell->back = frontTransitionNode;
	}
	else if( frontNortheastSubCell->front->type == 'w' ) {
		whiteNeighborCell = static_cast<TransitionNode*>(frontTransitionNode->singleConnector);

		if( whiteNeighborCell->singleConnector == frontNortheastSubCell )
			whiteNeighborCell->singleConnector = frontTransitionNode;

		else if( whiteNeighborCell->quadrupleConnector1 == frontNortheastSubCell )
				whiteNeighborCell->quadrupleConnector1 = frontTransitionNode;

		else if( whiteNeighborCell->quadrupleConnector2 == frontNortheastSubCell )
				whiteNeighborCell->quadrupleConnector2 = frontTransitionNode;

		else if( whiteNeighborCell->quadrupleConnector3 == frontNortheastSubCell )
				whiteNeighborCell->quadrupleConnector3 = frontTransitionNode;

		else if( whiteNeighborCell->quadrupleConnector4 == frontNortheastSubCell )
				whiteNeighborCell->quadrupleConnector4 = frontTransitionNode;
	}

	/*==========================================================================
	                          BACK WHITE NODE

	  Points the east neighboring cell to the transition node.

		                                  n             B: Bunch
		                                 /              n: neighboring cell
		                             w  w               w: white node
		                             | /
		                        w -- B -- w
		                           / |
		                          w  w

	==========================================================================*/
	if( backTransitionNode->singleConnector->type == 'b' ) {
		blackNeighborCell = static_cast<CellNode*>(backTransitionNode->singleConnector);
		blackNeighborCell->front = backTransitionNode;
	}
	else if( whiteNeighborCell->type == 'w' ) {
		whiteNeighborCell = static_cast<TransitionNode*>(backTransitionNode->singleConnector);

		if( whiteNeighborCell->singleConnector == frontNortheastSubCell )
			whiteNeighborCell->singleConnector = backTransitionNode;

		else if( whiteNeighborCell->quadrupleConnector1 == frontNortheastSubCell )
				whiteNeighborCell->quadrupleConnector1 = backTransitionNode;

		else if( whiteNeighborCell->quadrupleConnector2 == frontNortheastSubCell )
				whiteNeighborCell->quadrupleConnector2 = backTransitionNode;

		else if( whiteNeighborCell->quadrupleConnector3 == frontNortheastSubCell )
				whiteNeighborCell->quadrupleConnector3 = backTransitionNode;

		else if( whiteNeighborCell->quadrupleConnector4 == frontNortheastSubCell )
				whiteNeighborCell->quadrupleConnector4 = backTransitionNode;
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

	/*==========================================================================
	                    SIMPLIFICATION OF WHITE NODES

	If necessary, simplifies the graph by eliminating adjacent transition nodes
	of same level connected through their singleConnectors.
	==========================================================================*/
	simplifyGraph( eastTransitionNode  );
	simplifyGraph( northTransitionNode );
	simplifyGraph( westTransitionNode  );
	simplifyGraph( southTransitionNode );
	simplifyGraph( frontTransitionNode );
	simplifyGraph( backTransitionNode  );
}


/*==============================================================================
	                        FUNCTION simplify()

	Simplifies the data structure eliminating adjacent transition nodes
	of same level.

	C: Cell node.
	T: Transition node.


	Before:

	     C_____________C___                       ___C______________C
	    /|            /|   \                   _-   /|             /|
	  /  |          /  |    \                _-   /  |           /  |
	C/___|_______C/____|_    \             _- ___/___|________ /    |
	|    |       |     |  - _ \           -  /   C   |        C     |
	|    |       |     |     -_\         - /     |   |        |     |
	|    C_______|_____|_______T---------T_______|___|________|_____|
	|   /        |    /C    _-            \      |  /C        |    /C
	| /          |  /     _-               \     | /          |  /
	|/___________|/_____-                   \____|/___________|/
	C            C                               C            C


	After:
	             C_____________C_____________C_____________C
	            /|            /|            /|            /|
	          /  |          /  |          /  |          /  |
	        C/___|_______C/____|________C/___|_______C/    |
	        |    |       |     |        |    |       |     |
	        |    |       |     |        |    |       |     |
	        |    |_______|_____|________|____|_______|_____|
	        |   /C       |    /C        |   /        |    /C
	        |  /         |  /           |  /         |  /
	        |/___________|/_____________|/___________|/
	        C            C              C            C

==============================================================================*/
void Grid::simplifyGraph( TransitionNode *transitionNode ) throw (NullPointer) {
	if( transitionNode == 0 ) {
		throw NullPointer
		("Grid::simplifyGraph(): Parameter TransitionNode is NULL.");
	}

	// Pointers used to convert the Cell in a cell node or transition node.
	TransitionNode *neighborTransitionNode;
	CellNode *neighborCellNode;

	if( transitionNode->singleConnector != 0 ) {

		/* Both transition node and neighbor transition node have to have the
		 * ame refinement level. */
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
