/**
 * @file cell.cpp
 * File where the constructor of classes Cell, CellNode, and TransitionNode are
 * implemented.
 */

#include "cell.h"

/**
 * Constructor with default parameters of class Cell.
 *
 * @param type Type of cell.
 * @param active Indicates if this cell belongs to the three-dimensional domain.
 * @param level Refinement level of this cell.
 * @param centerX Coordinate X of the center of this cell.
 * @param centerY Coordinate Y of the center of this cell.
 * @param centerZ Coordinate Z of the center of this cell.
 */
Cell::Cell( char type     = 'x',
		bool active   = true,
		int level     = 1,
		double centerX = 0.0,
		double centerY = 0.0,
		double centerZ = 0.0
) {

	this->type    = type;
	this->active  = active;
	this->level   = level;
	this->centerX = centerX;
	this->centerY = centerY;
	this->centerZ = centerZ;
}

/**
 * Destructor of class Cell.
 */
Cell::~Cell() {}


/**
 * Constructor of class CellNode. All attributes of a new cell node are set to
 * zero, except sideLength, halfFaceLenth, and isToPaint, which are set to 1,
 * 0.5, and true, respectively.
 */
CellNode::CellNode(): Cell('b') {
	bunchNumber = 0;
	north       = 0;
	south       = 0;
	east        = 0;
	west        = 0;
	front       = 0;
	back        = 0;
	previous    = 0;
	next        = 0;

	gridPosition       = 0;
	hilbertShapeNumber = 0;
	faceLength         = 1;
	halfFaceLength     = 0.5;

	v         = 0;


	northFlux = 0;
	southFlux = 0;
	eastFlux  = 0;
	westFlux  = 0;
	frontFlux = 0;
	backFlux  = 0;

	b = 0.0;
	Ax = 0.0;
	r = 0.0;
	p = 0.0;
	p1 = 0.0;

	firstElement = new Element;
	sv = new double[NUMBER_EQUATIONS_CELL_MODEL];
};

/**
 * Destructor of class CEllNode.
 */
CellNode::~CellNode() {
    delete [] sv;    
    if (firstElement) {
        Element *aux = firstElement;
        while(aux) {
            Element *temp = aux;
            aux = aux->next;
            delete temp;
        }
    }
//    delete firstElement;

};

/**
 * Constructor of class TransitionNode All attributes of a new transition node
 * are set to zero.
 */
TransitionNode::TransitionNode(): Cell('w') {
	singleConnector     = 0;
	quadrupleConnector1 = 0;
	quadrupleConnector2 = 0;
	quadrupleConnector3 = 0;
	quadrupleConnector4 = 0;
	direction           = 0;
};

/**
 * Destructor of class TransitionNode.
 */
TransitionNode::~TransitionNode() {};
