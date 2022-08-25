#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../ALG/Grid/grid.h"
#include "Demo/demo.h"
#include "Display/display.h"

/*==============================================================================
                                     Controller
                                    
    This class is singleton. It means that it is impossible to create more than
    one object of this class.
==============================================================================*/
class Controller {
private:
    static Controller *controllerInstance;
	Controller(){}
    Controller(Controller &){}
    Controller& operator = (Controller &);
	  
public:
	static Controller *createController( Grid *grid );
	
	Grid *grid;
    Demo *demo;
    
    Display *display;
    
	bool  drawGrid;
	bool  drawGraph;
	bool  drawHilbertCurve;
	bool  writeCellsNumber;
	bool  writeDirections;
	bool  drawGridDomain;
		
	int   domainNumber;
	float domainParameter1;
	float domainParameter2;
	
	void reset();
};

#endif
