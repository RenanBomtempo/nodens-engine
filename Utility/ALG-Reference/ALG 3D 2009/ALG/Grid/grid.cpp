#include "grid.h"

/*==============================================================================
                                  Grid constructor.
==============================================================================*/
Grid::Grid() {

	// Cell nodes.
	CellNode *frontNortheastCell = new CellNode,
		 	 *frontNorthwestCell = new CellNode,
		 	 *frontSouthwestCell = new CellNode,
		 	 *frontSoutheastCell = new CellNode,
		 	 *backNortheastCell  = new CellNode,
		 	 *backNorthwestCell  = new CellNode,
		 	 *backSouthwestCell  = new CellNode,
		 	 *backSoutheastCell  = new CellNode;

	// Transition nodes.
	TransitionNode *northTransitionNode = new TransitionNode,
		 	       *southTransitionNode = new TransitionNode,
		 	       *eastTransitionNode  = new TransitionNode,
		 	       *westTransitionNode  = new TransitionNode,
		 	       *frontTransitionNode = new TransitionNode,
	 	 	       *backTransitionNode  = new TransitionNode;

	// Grid geometry.
	sideLength = 1.0;
	float halfSideLength    = sideLength / 2.0,
		   quarterSideLength = sideLength / 4.0;

	//__________________________________________________________________________
	//              Initialization of ground transition nodes.
	//__________________________________________________________________________
	// East transition node.
	eastTransitionNode->direction = 'e';
	eastTransitionNode->centerX   = halfSideLength;
	eastTransitionNode->centerY   = sideLength;
	eastTransitionNode->centerZ   = halfSideLength;
	eastTransitionNode->singleConnector     = 0;
	eastTransitionNode->quadrupleConnector1 = frontSoutheastCell;
	eastTransitionNode->quadrupleConnector2 = backSoutheastCell;
	eastTransitionNode->quadrupleConnector3 = backNortheastCell;
	eastTransitionNode->quadrupleConnector4 = frontNortheastCell;
	
	// North transition node.
	northTransitionNode->direction = 'n';
	northTransitionNode->centerX   = halfSideLength;
	northTransitionNode->centerY   = halfSideLength;
	northTransitionNode->centerZ   = sideLength;
	northTransitionNode->singleConnector     = 0;
	northTransitionNode->quadrupleConnector1 = frontNorthwestCell;
	northTransitionNode->quadrupleConnector2 = frontNortheastCell;
	northTransitionNode->quadrupleConnector3 = backNortheastCell;
	northTransitionNode->quadrupleConnector4 = backNorthwestCell;

	// West transition node.
	westTransitionNode->direction = 'w';
	westTransitionNode->centerX   = halfSideLength;
	westTransitionNode->centerY   = 0;
	westTransitionNode->centerZ   = halfSideLength;
	westTransitionNode->singleConnector     = 0;
	westTransitionNode->quadrupleConnector1 = frontSouthwestCell;
	westTransitionNode->quadrupleConnector2 = backSouthwestCell;
	westTransitionNode->quadrupleConnector3 = backNorthwestCell;
	westTransitionNode->quadrupleConnector4 = frontNorthwestCell;
	
	// South transition node.
	southTransitionNode->direction = 's';
	southTransitionNode->centerX   = halfSideLength;
	southTransitionNode->centerY   = halfSideLength;
	southTransitionNode->centerZ   = 0;
	southTransitionNode->singleConnector     = 0;
	southTransitionNode->quadrupleConnector1 = frontSouthwestCell;
	southTransitionNode->quadrupleConnector2 = frontSoutheastCell;
	southTransitionNode->quadrupleConnector3 = backSoutheastCell;
	southTransitionNode->quadrupleConnector4 = backSouthwestCell;
	
	// Front transition node.
	frontTransitionNode->direction = 'f';
	frontTransitionNode->centerX   = sideLength;
	frontTransitionNode->centerY   = halfSideLength;
	frontTransitionNode->centerZ   = halfSideLength;
	frontTransitionNode->singleConnector     = 0;
	frontTransitionNode->quadrupleConnector1 = frontSouthwestCell;
	frontTransitionNode->quadrupleConnector2 = frontSoutheastCell;
	frontTransitionNode->quadrupleConnector3 = frontNortheastCell;
	frontTransitionNode->quadrupleConnector4 = frontNorthwestCell;
	
	// Back transition node.
	backTransitionNode->direction = 'b';
	backTransitionNode->centerX   = 0;
	backTransitionNode->centerY   = halfSideLength;
	backTransitionNode->centerZ   = halfSideLength;
	backTransitionNode->singleConnector     = 0;
	backTransitionNode->quadrupleConnector1 = backSouthwestCell;
	backTransitionNode->quadrupleConnector2 = backSoutheastCell;
	backTransitionNode->quadrupleConnector3 = backNortheastCell;
	backTransitionNode->quadrupleConnector4 = backNorthwestCell;

	//__________________________________________________________________________
	//                      Initialization of cell nodes.
	//__________________________________________________________________________
	// front Northeast subcell initialization.
	frontNortheastCell->faceLength     = halfSideLength;
	frontNortheastCell->halfFaceLength = quarterSideLength;
	frontNortheastCell->bunchNumber    = 1;
	
	frontNortheastCell->east     = eastTransitionNode;
	frontNortheastCell->north    = northTransitionNode;
	frontNortheastCell->west     = frontNorthwestCell;
	frontNortheastCell->south    = frontSoutheastCell;
	frontNortheastCell->front    = frontTransitionNode;
	frontNortheastCell->back     = backNortheastCell;
	frontNortheastCell->previous = 0;
	frontNortheastCell->next     = backNortheastCell;

	frontNortheastCell->gridPosition       = 0;
	frontNortheastCell->hilbertShapeNumber = 1;

	frontNortheastCell->centerX = halfSideLength + quarterSideLength;
	frontNortheastCell->centerY = halfSideLength + quarterSideLength;
	frontNortheastCell->centerZ = halfSideLength + quarterSideLength;

	//__________________________________________________________________________
	// back Northeast subcell initialization.
	backNortheastCell->faceLength     = halfSideLength;
	backNortheastCell->halfFaceLength = quarterSideLength;
	backNortheastCell->bunchNumber    = 2;

	backNortheastCell->east     = eastTransitionNode;
	backNortheastCell->north    = northTransitionNode;
	backNortheastCell->west     = backNorthwestCell;
	backNortheastCell->south    = backSoutheastCell;
	backNortheastCell->front    = frontNortheastCell;
	backNortheastCell->back     = backTransitionNode;
	backNortheastCell->previous = frontNortheastCell;
	backNortheastCell->next     = backNorthwestCell;

	backNortheastCell->gridPosition       = 1;
	backNortheastCell->hilbertShapeNumber = 2;

	backNortheastCell->centerX = quarterSideLength;
	backNortheastCell->centerY = halfSideLength + quarterSideLength;
	backNortheastCell->centerZ = halfSideLength + quarterSideLength;

	//__________________________________________________________________________
	// back Northwest subcell initialization.
	backNorthwestCell->faceLength     = halfSideLength;
	backNorthwestCell->halfFaceLength = quarterSideLength;
	backNorthwestCell->bunchNumber    = 3;
	
	backNorthwestCell->east     = backNortheastCell;
	backNorthwestCell->north    = northTransitionNode;
	backNorthwestCell->west     = westTransitionNode;
	backNorthwestCell->south    = backSouthwestCell;
	backNorthwestCell->front    = frontNorthwestCell;
	backNorthwestCell->back     = backTransitionNode;
	backNorthwestCell->previous = backNortheastCell;
	backNorthwestCell->next     = frontNorthwestCell;

	backNorthwestCell->gridPosition       = 2;
	backNorthwestCell->hilbertShapeNumber = 2;

	backNorthwestCell->centerX = quarterSideLength;
	backNorthwestCell->centerY = quarterSideLength;
	backNorthwestCell->centerZ = halfSideLength + quarterSideLength;

	//__________________________________________________________________________
	// front Northwest subcell initialization.
	frontNorthwestCell->faceLength     = halfSideLength;
	frontNorthwestCell->halfFaceLength = quarterSideLength;
	frontNorthwestCell->bunchNumber    = 4;

	frontNorthwestCell->east     = frontNortheastCell;
	frontNorthwestCell->north    = northTransitionNode;
	frontNorthwestCell->west     = westTransitionNode;
	frontNorthwestCell->south    = frontSouthwestCell;
	frontNorthwestCell->front    = frontTransitionNode;
	frontNorthwestCell->back     = backNorthwestCell;
	frontNorthwestCell->previous = backNorthwestCell;
	frontNorthwestCell->next     = frontSouthwestCell;

	frontNorthwestCell->gridPosition       = 3;
	frontNorthwestCell->hilbertShapeNumber = 3;

	frontNorthwestCell->centerX = halfSideLength + quarterSideLength;
	frontNorthwestCell->centerY = quarterSideLength;
	frontNorthwestCell->centerZ = halfSideLength + quarterSideLength;

	//__________________________________________________________________________
	// front Southwest subcell initialization.
	frontSouthwestCell->faceLength     = halfSideLength;
	frontSouthwestCell->halfFaceLength = quarterSideLength;
	frontSouthwestCell->bunchNumber    = 5;
	
	frontSouthwestCell->east     = frontSoutheastCell;
	frontSouthwestCell->north    = frontNorthwestCell;
	frontSouthwestCell->west     = westTransitionNode;
	frontSouthwestCell->south    = southTransitionNode;
	frontSouthwestCell->front    = frontTransitionNode;
	frontSouthwestCell->back     = backSouthwestCell;
	frontSouthwestCell->previous = frontNorthwestCell;
	frontSouthwestCell->next     = backSouthwestCell;
	
	frontSouthwestCell->gridPosition       = 4;
	frontSouthwestCell->hilbertShapeNumber = 3;
	
	frontSouthwestCell->centerX = halfSideLength + quarterSideLength;
	frontSouthwestCell->centerY = quarterSideLength;
	frontSouthwestCell->centerZ = quarterSideLength;
	
	//__________________________________________________________________________
	// back Southwest subcell initialization.
	backSouthwestCell->faceLength     = halfSideLength;
	backSouthwestCell->halfFaceLength = quarterSideLength;
	backSouthwestCell->bunchNumber    = 6;
	
	backSouthwestCell->east     = backSoutheastCell;
	backSouthwestCell->north    = backNorthwestCell;
	backSouthwestCell->west     = westTransitionNode;
	backSouthwestCell->south    = southTransitionNode;
	backSouthwestCell->front    = frontSouthwestCell;
	backSouthwestCell->back     = backTransitionNode;
	backSouthwestCell->previous = frontSouthwestCell;
	backSouthwestCell->next     = backSoutheastCell;

	backSouthwestCell->gridPosition       = 5;
	backSouthwestCell->hilbertShapeNumber = 4;

	backSouthwestCell->centerX = quarterSideLength;
	backSouthwestCell->centerY = quarterSideLength;
	backSouthwestCell->centerZ = quarterSideLength;

	//__________________________________________________________________________
	// back Southeast subcell initialization.
	backSoutheastCell->faceLength     = halfSideLength;
	backSoutheastCell->halfFaceLength = quarterSideLength;
	backSoutheastCell->bunchNumber    = 7;
	
	backSoutheastCell->east     = eastTransitionNode;
	backSoutheastCell->north    = backNortheastCell;
	backSoutheastCell->west     = backSouthwestCell;
	backSoutheastCell->south    = southTransitionNode;
	backSoutheastCell->front    = frontSoutheastCell;
	backSoutheastCell->back     = backTransitionNode;
	backSoutheastCell->previous = backSouthwestCell;
	backSoutheastCell->next     = frontSoutheastCell;

	backSoutheastCell->gridPosition       = 6;
	backSoutheastCell->hilbertShapeNumber = 4;

	backSoutheastCell->centerX = quarterSideLength;
	backSoutheastCell->centerY = halfSideLength + quarterSideLength;
	backSoutheastCell->centerZ = quarterSideLength;

	//__________________________________________________________________________
	// front Southeast subcell initialization.
	frontSoutheastCell->faceLength     = halfSideLength;
	frontSoutheastCell->halfFaceLength = quarterSideLength;
	frontSoutheastCell->bunchNumber    = 8;

	frontSoutheastCell->east     = eastTransitionNode;
	frontSoutheastCell->north    = frontNortheastCell;
	frontSoutheastCell->west     = frontSouthwestCell;
	frontSoutheastCell->south    = southTransitionNode;
	frontSoutheastCell->front    = frontTransitionNode;
	frontSoutheastCell->back     = backSoutheastCell;
	frontSoutheastCell->previous = backSoutheastCell;
	frontSoutheastCell->next     = 0;

	frontSoutheastCell->gridPosition       = 7;
	frontSoutheastCell->hilbertShapeNumber = 5;

	frontSoutheastCell->centerX = halfSideLength + quarterSideLength;
	frontSoutheastCell->centerY = halfSideLength + quarterSideLength;
	frontSoutheastCell->centerZ = quarterSideLength;

	//__________________________________________________________________________
	// Grid initialization
	firstCell = frontNortheastCell;
	numberOfCells = 8;
}

/*==============================================================================
                                Grid destructor.
==============================================================================*/
Grid::~Grid() {
	// Before release the area allocated, the grid is derefined until level 1.
	// Thus, the grid shape is known and each node can be easily reached.
	while( numberOfCells > 8 ) {
		   this->derefineAll();
 	}
	
	CellNode *gridCell = firstCell;
	
	// Eliminating North transition node.
	delete static_cast<TransitionNode*>(gridCell->north);        

	// Eliminating Front transition node.
	delete static_cast<TransitionNode*>(gridCell->front);        

	// Eliminating East transiton node.
	delete static_cast<TransitionNode*>(gridCell->east);         

	// Eliminating West Transiton node.
	delete static_cast<TransitionNode*>(static_cast<CellNode*>(gridCell->west)->west);   

	// Eliminating South transition node.
	delete static_cast<TransitionNode*>(static_cast<CellNode*>(gridCell->south)->south); 

	// Eliminating Back transition node.
	delete static_cast<TransitionNode*>(static_cast<CellNode*>(gridCell->back)->back);   
	
	// Eliminating Black Cells.
	gridCell = gridCell->next;
	while( gridCell->next != 0 ) {
		   delete gridCell->previous;
		   gridCell = gridCell->next;
 	}
 	delete gridCell;
}

/*==============================================================================
                               getNumberOfCells()
 
    Returns the number of black cells.
==============================================================================*/
long int Grid::getNumberOfCells() {
    return numberOfCells;
}

/*==============================================================================
                                 orderGridCells()

    Enumerates cells of grid according to the ordering given by Hilbert's curve.
==============================================================================*/
void Grid::orderGridCells() {
	CellNode *gridCell;
	gridCell = firstCell;

	long int counter = 0;
	while( gridCell != 0 ) {
		gridCell->gridPosition = counter;
		gridCell = gridCell->next;
		counter += 1;
	}
}
