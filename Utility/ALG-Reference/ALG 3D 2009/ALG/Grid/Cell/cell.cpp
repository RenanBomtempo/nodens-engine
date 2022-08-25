#include "cell.h"

/*==============================================================================
                   Cell Constructor with default parameters
==============================================================================*/
Cell::Cell( char type     = 'x',
			bool active   = true,
			int level     = 1,
			float centerX = 0.0,
			float centerY = 0.0,
			float centerZ = 0.0
		  ) {
					  
    this->type    = type;
	this->active  = active;
	this->level   = level;
	this->centerX = centerX;
	this->centerY = centerY;
	this->centerZ = centerZ;
}

/*==============================================================================
                               Cell Destructor
==============================================================================*/
Cell::~Cell() {}


/*==============================================================================
                            Cell Node Constructor
==============================================================================*/
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
	
	u         = 0;
	previousU = 0;
	
	isToPaint = true;
	
	northFlux = 0;
	southFlux = 0;
	eastFlux  = 0;
	westFlux  = 0;
	frontFlux = 0;
	backFlux  = 0;
};

/*==============================================================================
                            Cell Node Destructor
==============================================================================*/
CellNode::~CellNode() {};

/*==============================================================================
                           Transition Node Constructor
==============================================================================*/
TransitionNode::TransitionNode(): Cell('w') {
	singleConnector     = 0;
	quadrupleConnector1 = 0;
	quadrupleConnector2 = 0;
	quadrupleConnector3 = 0;
	quadrupleConnector4 = 0;
	direction           = 0;
};

/*==============================================================================
                            Transition Node Destructor
==============================================================================*/
TransitionNode::~TransitionNode() {};
