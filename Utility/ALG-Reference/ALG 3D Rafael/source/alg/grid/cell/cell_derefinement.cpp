/**
 * @file cell_derefinement.cpp
 * File where all methods of class CellNode which are related to cell
 * derefinement are implemented.
 */

#include "../grid.h"
#include <iostream>

/**
 * Derefines a bunch of seven cells  transforming it into a unique cell. It
 * receives as argument the first cell of the bunch which will be derefined.
 * If the argument is not the first cell, the behavior of this function and
 * its result is undefined.
 *
 * @param firstBunchCell First cell of the bunch to be derefined.
 * @throw NullPointer If a null cell node is given as argument, a NullPointer
 * exception is thrown.
 */
void Grid::derefineBunch( CellNode *firstBunchCell ) throw (NullPointer) {
	if( firstBunchCell == 0 ) {
		throw NullPointer
		("Grid::derefineBunch(): Parameter firstBunchCell is NULL.");
	}

	CellNode *cellBeforeBunch = firstBunchCell->previous;
	CellNode *cellAfterBunch  = firstBunchCell->next->next->next->next->next->next->next->next;

	int bunchLevel         = firstBunchCell->level;
	int hilbertShapeNumber = getFatherBunchNumber( firstBunchCell );
	int bunchNumber        = firstBunchCell->bunchNumber;

	// New cell variable (Arithmetic mean between all cells of the bunch).
	double u = 0,
			previousU = 0;

//	double *new_sv = new double[NUMBER_EQUATIONS_CELL_MODEL];
//	int j;
//	for (j = 0; j < NUMBER_EQUATIONS_CELL_MODEL; ++j) {
//		new_sv[j] = 0.0;
//	}


	CellNode* auxiliar = firstBunchCell;
	for( int i = 0; i < 8; i++ ) {
		u += auxiliar->v;
//		previousU += auxiliar->previousU;
//		for (j = 0; j < NUMBER_EQUATIONS_CELL_MODEL; ++j) {
//			new_sv[j] += auxiliar->sv[j];
//		}

		auxiliar = auxiliar->next;
	}
	u /= 8.0;
//	previousU /= 8.0;

	// Front northeast node of the bunch becomes the derefined node.
	CellNode *newCell = getFrontNortheastCell( firstBunchCell );
	newCell->previous  = cellBeforeBunch;
	newCell->next      = cellAfterBunch;

	if( newCell->previous != 0 )
		newCell->previous->next = newCell;
	if( newCell->next != 0 )
		newCell->next->previous = newCell;

	// New geometric variables.
	newCell->centerX = ( newCell->centerX + newCell->back->centerX ) / 2.0;
	newCell->centerY = ( newCell->centerY + newCell->west->centerY ) / 2.0;
	newCell->centerZ = ( newCell->centerZ + newCell->south->centerZ ) / 2.0;

	newCell->faceLength     = 2 * newCell->faceLength;
	newCell->halfFaceLength = 2 * newCell->halfFaceLength;
	newCell->v              = u;
	newCell->previousU      = previousU;

//	for (j = 0; j < NUMBER_EQUATIONS_CELL_MODEL; ++j) {
//		newCell->sv[j] = new_sv[j]/8.0;
//	}
//
//	delete [] new_sv;

	newCell->level              = bunchLevel - 1;
	newCell->hilbertShapeNumber = hilbertShapeNumber;
	newCell->bunchNumber        = bunchNumber / (int)10;

	CellNode *frontNortheastCell = newCell;
	CellNode *frontSoutheastCell = static_cast<CellNode*>(frontNortheastCell->south);
	CellNode *frontNorthwestCell = static_cast<CellNode*>(frontNortheastCell->west);
	CellNode *frontSouthwestCell = static_cast<CellNode*>(frontNorthwestCell->south);
	CellNode *backNortheastCell  = static_cast<CellNode*>(frontNortheastCell->back);
	CellNode *backSoutheastCell  = static_cast<CellNode*>(backNortheastCell->south);
	CellNode *backNorthwestCell  = static_cast<CellNode*>(backNortheastCell->west);
	CellNode *backSouthwestCell  = static_cast<CellNode*>(backNorthwestCell->south);

	// Creation of North Transition Node.
	TransitionNode *northTransitionNode = new TransitionNode;
	northTransitionNode->type      = 'w';
	northTransitionNode->level     = bunchLevel;
	northTransitionNode->direction = 'n';
	northTransitionNode->active    = newCell->active;
	northTransitionNode->centerX   = newCell->centerX;
	northTransitionNode->centerY   = newCell->centerY;
	northTransitionNode->centerZ   = newCell->centerZ + newCell->halfFaceLength;
	northTransitionNode->singleConnector     = newCell;
	northTransitionNode->quadrupleConnector1 = frontNorthwestCell->north;
	northTransitionNode->quadrupleConnector2 = frontNortheastCell->north;
	northTransitionNode->quadrupleConnector3 = backNortheastCell->north;
	northTransitionNode->quadrupleConnector4 = backNorthwestCell->north;

	// Creation of South Transition Node.
	TransitionNode *southTransitionNode = new TransitionNode;
	southTransitionNode->type      = 'w';
	southTransitionNode->level     = bunchLevel;
	southTransitionNode->direction = 's';
	southTransitionNode->active    = newCell->active;
	southTransitionNode->centerX   = newCell->centerX;
	southTransitionNode->centerY   = newCell->centerY;
	southTransitionNode->centerZ   = newCell->centerZ - newCell->halfFaceLength;
	southTransitionNode->singleConnector     = newCell;
	southTransitionNode->quadrupleConnector1 = frontSouthwestCell->south;
	southTransitionNode->quadrupleConnector2 = frontSoutheastCell->south;
	southTransitionNode->quadrupleConnector3 = backSoutheastCell->south;
	southTransitionNode->quadrupleConnector4 = backSouthwestCell->south;

	// Creation of East Transition Node.
	TransitionNode *eastTransitionNode = new TransitionNode;
	eastTransitionNode->type      = 'w';
	eastTransitionNode->level     = bunchLevel;
	eastTransitionNode->direction = 'e';
	eastTransitionNode->active    = newCell->active;
	eastTransitionNode->centerX   = newCell->centerX;
	eastTransitionNode->centerY   = newCell->centerY + newCell->halfFaceLength;
	eastTransitionNode->centerZ   = newCell->centerZ;
	eastTransitionNode->singleConnector     = newCell;
	eastTransitionNode->quadrupleConnector1 = frontSoutheastCell->east;
	eastTransitionNode->quadrupleConnector2 = backSoutheastCell->east;
	eastTransitionNode->quadrupleConnector3 = backNortheastCell->east;
	eastTransitionNode->quadrupleConnector4 = frontNortheastCell->east;

	// Creation of West Transition Node.
	TransitionNode *westTransitionNode = new TransitionNode;
	westTransitionNode->type      = 'w';
	westTransitionNode->level     = bunchLevel;
	westTransitionNode->direction = 'w';
	westTransitionNode->active    = newCell->active;
	westTransitionNode->centerX   = newCell->centerX;
	westTransitionNode->centerY   = newCell->centerY - newCell->halfFaceLength;
	westTransitionNode->centerZ   = newCell->centerZ;
	westTransitionNode->singleConnector     = newCell;
	westTransitionNode->quadrupleConnector1 = frontSouthwestCell->west;
	westTransitionNode->quadrupleConnector2 = backSouthwestCell->west;
	westTransitionNode->quadrupleConnector3 = backNorthwestCell->west;
	westTransitionNode->quadrupleConnector4 = frontNorthwestCell->west;

	// Creation of Front Transition Node.
	TransitionNode *frontTransitionNode = new TransitionNode;
	frontTransitionNode->type      = 'w';
	frontTransitionNode->level     = bunchLevel;
	frontTransitionNode->direction = 'f';
	frontTransitionNode->active    = newCell->active;
	frontTransitionNode->centerX   = newCell->centerX + newCell->halfFaceLength;
	frontTransitionNode->centerY   = newCell->centerY;
	frontTransitionNode->centerZ   = newCell->centerZ;
	frontTransitionNode->singleConnector     = newCell;
	frontTransitionNode->quadrupleConnector1 = frontSouthwestCell->front;
	frontTransitionNode->quadrupleConnector2 = frontSoutheastCell->front;
	frontTransitionNode->quadrupleConnector3 = frontNortheastCell->front;
	frontTransitionNode->quadrupleConnector4 = frontNorthwestCell->front;

	// Creation of Back Transition Node.
	TransitionNode *backTransitionNode = new TransitionNode;
	backTransitionNode->type      = 'w';
	backTransitionNode->level     = bunchLevel;
	backTransitionNode->direction = 'b';
	backTransitionNode->active    = newCell->active;
	backTransitionNode->centerX   = newCell->centerX - newCell->halfFaceLength;
	backTransitionNode->centerY   = newCell->centerY;
	backTransitionNode->centerZ   = newCell->centerZ;
	backTransitionNode->singleConnector     = newCell;
	backTransitionNode->quadrupleConnector1 = backSouthwestCell->back;
	backTransitionNode->quadrupleConnector2 = backSoutheastCell->back;
	backTransitionNode->quadrupleConnector3 = backNortheastCell->back;
	backTransitionNode->quadrupleConnector4 = backNorthwestCell->back;

	// Elimination of the seven unneeded bunch cells.
	delete frontNorthwestCell;
	delete frontSouthwestCell;
	delete frontSoutheastCell;
	delete backNortheastCell;
	delete backNorthwestCell;
	delete backSoutheastCell;
	delete backSouthwestCell;

	// Linking of derefined cell and new transition nodes.
	newCell->north = northTransitionNode;
	newCell->south = southTransitionNode;
	newCell->east  = eastTransitionNode;
	newCell->west  = westTransitionNode;
	newCell->front = frontTransitionNode;
	newCell->back  = backTransitionNode;

	// Simplification of grid Eliminating unneeded transition nodes.
	simplifyDeref( northTransitionNode );
	simplifyDeref( southTransitionNode );
	simplifyDeref( eastTransitionNode );
	simplifyDeref( westTransitionNode );
	simplifyDeref( frontTransitionNode );
	simplifyDeref( backTransitionNode );
}


/**
 * Simplifies the graph by eliminating adjacent transition nodes of same  level
 * connected through their quadruple connectors. If a transition  node  is  not
 * eliminated, it's  because there is no adjacent transition node of same level,
 * then simply connects the outside to it.
 *
 * @param transitionNode Candidate transition node to be eliminated.
 * @throw NullPointer If a null transition node is given as argument, a NullPointer
 * exception is thrown.
 */
void Grid::simplifyDeref( TransitionNode *transitionNode ) throw (NullPointer) {
	if( transitionNode == 0 ) {
		throw NullPointer
		("Grid::clean(): Parameter transitionNode is NULL.");
	}

	CellNode *derefinedCell = static_cast<CellNode*>(transitionNode->singleConnector);
	char direction = transitionNode->direction;

	TransitionNode *whiteNeighborCell;
	CellNode *blackNeighborCell;
	TransitionNode *neighborTransitionNode;
	Cell *quadrupleConnector[4];

	/* All quadruple connectors point to the  same  cell.  It  means  that  two
	 * transition nodes of the same level have been connected. This connection is
	 * not allowed and it is simplified through a direct connection between the two
	 * cell nodes pointed by each transition node. Then, both transition nodes
	 * are eliminated.
	 */
	if( ( transitionNode->quadrupleConnector1 == transitionNode->quadrupleConnector2 )
			&& ( transitionNode->quadrupleConnector1 == transitionNode->quadrupleConnector3 )
			&& ( transitionNode->quadrupleConnector1 == transitionNode->quadrupleConnector4 ) )
	{
		// PS: No matter which quadruple connector is chosen in the line below,
		// because each one of them points to the same cell.
		neighborTransitionNode = static_cast<TransitionNode*>(transitionNode->quadrupleConnector1);
		char neighborCellType  = neighborTransitionNode->singleConnector->type;

		switch( direction ) {
		case 'n':{ derefinedCell->north = neighborTransitionNode->singleConnector; break; }
		case 's':{ derefinedCell->south = neighborTransitionNode->singleConnector; break; }
		case 'e':{ derefinedCell->east  = neighborTransitionNode->singleConnector; break; }
		case 'w':{ derefinedCell->west = neighborTransitionNode->singleConnector;  break; }
		case 'f':{ derefinedCell->front = neighborTransitionNode->singleConnector; break; }
		case 'b':{ derefinedCell->back = neighborTransitionNode->singleConnector;  break; }
		default:{ break; }
		}

		if( neighborCellType == 'b' ) {
			blackNeighborCell = static_cast<CellNode*>(neighborTransitionNode->singleConnector);
			switch( direction ) {
			case 'n':{ blackNeighborCell->south = derefinedCell; break; }
			case 's':{ blackNeighborCell->north = derefinedCell; break; }
			case 'e':{ blackNeighborCell->west  = derefinedCell; break; }
			case 'w':{ blackNeighborCell->east  = derefinedCell; break; }
			case 'f':{ blackNeighborCell->back  = derefinedCell; break; }
			case 'b':{ blackNeighborCell->front = derefinedCell; break; }
			default: { break;}
			}
		}
		else {
			whiteNeighborCell = static_cast<TransitionNode*>(neighborTransitionNode->singleConnector);

			if( whiteNeighborCell->singleConnector == neighborTransitionNode )
				whiteNeighborCell->singleConnector = derefinedCell;

			else if( whiteNeighborCell->quadrupleConnector1	== neighborTransitionNode )
				whiteNeighborCell->quadrupleConnector1 = derefinedCell;

			else if( whiteNeighborCell->quadrupleConnector2	== neighborTransitionNode )
				whiteNeighborCell->quadrupleConnector2 = derefinedCell;

			else if( whiteNeighborCell->quadrupleConnector3	== neighborTransitionNode )
				whiteNeighborCell->quadrupleConnector3 = derefinedCell;

			else if( whiteNeighborCell->quadrupleConnector4	== neighborTransitionNode )
				whiteNeighborCell->quadrupleConnector4 = derefinedCell;
		}
		delete neighborTransitionNode;
		delete transitionNode;
	}

	/* Connects outside to the transition node, if this was not deleted. That is,
	 * the quadruple connectors point to different cells. */
	else {
		quadrupleConnector[0] = transitionNode->quadrupleConnector1;
		quadrupleConnector[1] = transitionNode->quadrupleConnector2;
		quadrupleConnector[2] = transitionNode->quadrupleConnector3;
		quadrupleConnector[3] = transitionNode->quadrupleConnector4;

		for( int i = 0; i < 4; i++ ) {
			if( quadrupleConnector[i]->type == 'w' ) {
				whiteNeighborCell = dynamic_cast<TransitionNode*>(quadrupleConnector[i]);
				whiteNeighborCell->singleConnector = transitionNode;
			}
			else if( quadrupleConnector[i]->type == 'b' ) {
				blackNeighborCell = dynamic_cast<CellNode*>(quadrupleConnector[i]);
				switch( direction ) {
				case 'n':{ blackNeighborCell->south = transitionNode; break; }
				case 's':{ blackNeighborCell->north = transitionNode; break; }
				case 'e':{ blackNeighborCell->west  = transitionNode; break; }
				case 'w':{ blackNeighborCell->east  = transitionNode; break; }
				case 'f':{ blackNeighborCell->back  = transitionNode; break; }
				case 'b':{ blackNeighborCell->front = transitionNode; break; }
				default: { break; }
				}
			}
		}
	}
}

/**
 * Gets a pointer to the frontNortheast cell of a bunch. In order to get this
 * cell, the first cell of the bunch has to be given as argument. If this cell is
 * not the first cell, the behavior of this function and its result is undefined.
 *
 * @param firstBunchCell First cell of the bunch where the front northeast cell
 * will be returned.
 * @return The front northeast cell of a bunch.
 * @throw NullPointer If a null cell node is given as argument, a NullPointer
 * exception is thrown.
 */
CellNode* Grid::getFrontNortheastCell( CellNode *firstBunchCell ) throw (NullPointer) {
	if( firstBunchCell == 0 ) {
		throw NullPointer
		("Grid::getFrontNortheastCell(): Parameter gridCell is NULL.");
	}

	CellNode *firstCell   = firstBunchCell;
	CellNode *secondCell  = firstCell->next;
	CellNode *thirdCell   = secondCell->next;
	CellNode *fourthCell  = thirdCell->next;
	CellNode *fifthCell   = fourthCell->next;
	CellNode *sixthCell   = fifthCell->next;
	CellNode *seventhCell = sixthCell->next;
	CellNode *eighthCell  = seventhCell->next;

	double coordinateSum1 = firstCell->centerX   + firstCell->centerY   + firstCell->centerZ;
	double coordinateSum2 = secondCell->centerX  + secondCell->centerY  + secondCell->centerZ;
	double coordinateSum3 = thirdCell->centerX   + thirdCell->centerY   + thirdCell->centerZ;
	double coordinateSum4 = fourthCell->centerX  + fourthCell->centerY  + fourthCell->centerZ;
	double coordinateSum5 = fifthCell->centerX   + fifthCell->centerY   + fifthCell->centerZ;
	double coordinateSum6 = sixthCell->centerX   + sixthCell->centerY   + sixthCell->centerZ;
	double coordinateSum7 = seventhCell->centerX + seventhCell->centerY + seventhCell->centerZ;
	double coordinateSum8 = eighthCell->centerX  + eighthCell->centerY  + eighthCell->centerZ;

	double maximum;
	CellNode *frontNortheastCell = firstCell;
	maximum = coordinateSum1;
	if( coordinateSum2 > maximum ) {
		maximum = coordinateSum2;
		frontNortheastCell = secondCell;
	}
	if( coordinateSum3 > maximum ) {
		maximum = coordinateSum3;
		frontNortheastCell = thirdCell;
	}
	if( coordinateSum4 > maximum ) {
		maximum = coordinateSum4;
		frontNortheastCell = fourthCell;
	}
	if( coordinateSum5 > maximum ) {
		maximum = coordinateSum5;
		frontNortheastCell = fifthCell;
	}
	if( coordinateSum6 > maximum ) {
		maximum = coordinateSum6;
		frontNortheastCell = sixthCell;
	}
	if( coordinateSum7 > maximum ) {
		maximum = coordinateSum7;
		frontNortheastCell = seventhCell;
	}
	if( coordinateSum8 > maximum ) {
		maximum = coordinateSum8;
		frontNortheastCell = eighthCell;
	}
	return frontNortheastCell;
}

/**
 * Gets the father's bunch number of a bunch, that is, the basic hilbert shape
 * number of a bunch. It is useful mostly on derefinement procedure, where a group
 * of eight cells become a single cell with the same information of the group's
 * father. In order to get the basic hilbert shape number of a bunch, its first
 * cell  has to be given as argument. If the argument is not the first cell, the
 * value -1 is returned.
 *
 * @param firstBunchCell First cell of the bunch where the basic hilbert shape
 * number will be returned.
 * @return The hilbert shape number of a bunch.
 * @throw NullPointer If a null cell node is given as argument, a NullPointer
 * exception is thrown.
 */
int Grid::getFatherBunchNumber( CellNode *firstBunchCell ) throw (NullPointer) {
	if( firstBunchCell == 0 ) {
		throw NullPointer
		("Grid::getFatherBunchNumber(): Parameter cell is NULL.");
	}

	CellNode *firstCell   = firstBunchCell;
	CellNode *secondCell  = firstCell->next;
	CellNode *thirdCell   = secondCell->next;
	CellNode *fourthCell  = thirdCell->next;
	CellNode *fifthCell   = fourthCell->next;
	CellNode *sixthCell   = fifthCell->next;
	CellNode *seventhCell = sixthCell->next;
	CellNode *eighthCell  = seventhCell->next;

	int hilbertShapeNumber = 0;
	int sumOfHilbertShapeNumbers = firstCell->hilbertShapeNumber
			+ secondCell->hilbertShapeNumber
			+ thirdCell->hilbertShapeNumber
			+ fourthCell->hilbertShapeNumber
			+ fifthCell->hilbertShapeNumber
			+ sixthCell->hilbertShapeNumber
			+ seventhCell->hilbertShapeNumber
			+ eighthCell->hilbertShapeNumber;

	switch( sumOfHilbertShapeNumbers ) {
	case 24: {
		hilbertShapeNumber = 0;
		break;
	}
	case 38: {
		hilbertShapeNumber = 1;
		break;
	}
	case 50: {
		hilbertShapeNumber = 2;
		break;
	}
	case 49: {
		hilbertShapeNumber = 3;
		break;
	}
	case 45: {
		if( secondCell->hilbertShapeNumber == 10 )
			hilbertShapeNumber = 4;

		else if( secondCell->hilbertShapeNumber == 4 )
			hilbertShapeNumber = 10;

		else if( secondCell->hilbertShapeNumber == 3 )
			hilbertShapeNumber = 11;

		break;
	}
	case 56: {
		hilbertShapeNumber = 5;
		break;
	}
	case 41: {
		hilbertShapeNumber = 6;
		break;
	}
	case 43: {
		hilbertShapeNumber = 7;
		break;
	}
	case 58: {
		hilbertShapeNumber = 8;
		break;
	}
	case 37: {
		hilbertShapeNumber = 9;
		break;
	}
	default: {
		hilbertShapeNumber = -1;
		break;
	}
	}
	return hilbertShapeNumber;
}

/**
 * Decides if the bunch should be derefined. A bunch will not be derefined if
 * the highest of all directional fluxes coming into its  six  directions  is
 * greater than derefinementBound.
 *
 * @param gridCell Cell to be verified if it needs to be derefined.
 * @param dererefinementBound Maximum flux allowed to this cell.
 * @throw NullPointer If a null cell node is given as argument, a NullPointer
 * exception is thrown.
 */
bool Grid::needsDerefinement( CellNode *gridCell, double derefinementBound ) throw (NullPointer) {
	if( gridCell == 0 ) {
		throw NullPointer
		("Grid::needsDerefinement(): Parameter gridCell is NULL.");
	}

	CellNode *firstCell   = gridCell;
	CellNode *secondCell  = firstCell->next;
	CellNode *thirdCell   = secondCell->next;
	CellNode *fourthCell  = thirdCell->next;
	CellNode *fifthCell   = fourthCell->next;
	CellNode *sixthCell   = fifthCell->next;
	CellNode *seventhCell = sixthCell->next;
	CellNode *eighthCell  = seventhCell->next;

	double maximum1 = firstCell->getMaximumFlux();
	double maximum2 = secondCell->getMaximumFlux();
	double maximum3 = thirdCell->getMaximumFlux();
	double maximum4 = fourthCell->getMaximumFlux();
	double maximum5 = fifthCell->getMaximumFlux();
	double maximum6 = sixthCell->getMaximumFlux();
	double maximum7 = seventhCell->getMaximumFlux();
	double maximum8 = eighthCell->getMaximumFlux();

	double highestMaximum = maximum1;
	if( maximum2 > highestMaximum )
		highestMaximum = maximum2;

	if( maximum3 > highestMaximum )
		highestMaximum = maximum3;

	if( maximum4 > highestMaximum )
		highestMaximum = maximum4;

	if( maximum5 > highestMaximum )
		highestMaximum = maximum5;

	if( maximum6 > highestMaximum )
		highestMaximum = maximum6;

	if( maximum7 > highestMaximum )
		highestMaximum = maximum7;

	if( maximum8 > highestMaximum )
		highestMaximum = maximum8;

	bool derefinementCondition = false;

	if( highestMaximum <= derefinementBound )
		derefinementCondition = true;

	return derefinementCondition;
}
