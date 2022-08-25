#include "controller.h"

/*==============================================================================
                        Controller instance initilization
==============================================================================*/
Controller* Controller::controllerInstance = 0;


/*==============================================================================
                          FUNCTION createController()

    It creates only one object controller. If this function is called more than
	once, the first object created is returned.
==============================================================================*/
Controller *Controller::createController( Grid *grid ) {
    if( controllerInstance == 0 ) {
		controllerInstance = new Controller;
		
		controllerInstance->grid    = grid;
		controllerInstance->demo    = 0;
		controllerInstance->display = 0;
		
		controllerInstance->drawGrid         = true;
		controllerInstance->drawGraph        = false;
		controllerInstance->drawHilbertCurve = false;
		controllerInstance->writeCellsNumber = false;
		controllerInstance->writeDirections  = false;
		controllerInstance->drawGridDomain   = false;
	
		controllerInstance->domainNumber     = 0;
		controllerInstance->domainParameter1 = 0.3;
		controllerInstance->domainParameter2 = 0.5;
	}
	return controllerInstance;
}


/*==============================================================================
                               FUNCTION reset()

    Resets all variables to their default values, except the pointers to the
    grid, demo, and display.
==============================================================================*/
void Controller::reset() {
		drawGrid         = false;
		drawGraph        = false;
		drawHilbertCurve = false;
		writeCellsNumber = false;
		writeDirections  = false;
		drawGridDomain   = false;

		domainNumber     = 0;
		domainParameter1 = 0.3;
		domainParameter2 = 0.5;
}
