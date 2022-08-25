#include "demo.h"
#include "../controller.h"


/*==============================================================================
                    Intitialization of demo instance.
==============================================================================*/
Demo *Demo::demoInstance = 0;

/*==============================================================================
                             FUNCTION createDemo()
==============================================================================*/
Demo *Demo::createDemo( Controller *controller ) {
	 if( demoInstance == 0 ) {
	 	 demoInstance               = new Demo();
	 	 demoInstance->running      = false;
	 	 demoInstance->currentState = 0;
	 	 demoInstance->controller   = controller;
	 	 controller->demo           = demoInstance;
	 }
	 return demoInstance;
}

/*==============================================================================
                             FUNCTION start()
                            
   Starts demostration from the initial state.    
==============================================================================*/
void Demo::start() {
	 running      = true;
	 currentState = 0;
}

/*==============================================================================
                              FUNCTION stop()
                            
   Stops demostration and goes to the initial state.      
==============================================================================*/
void Demo::stop() {
	 running      = false;
	 currentState = 0;
}

/*==============================================================================
                             FUNCTION pause()
                            
   Pauses demostration on current state.           
==============================================================================*/
void Demo::pause() {
	 running = false;
}

/*==============================================================================
                             FUNCTION restart()
                            
   Restarts demonstration from the initial state.            
==============================================================================*/
void Demo::restart() {
	 start();
}

/*==============================================================================
                             FUNCTION replay()
                            
   Resumes demonstration from the current state.           
==============================================================================*/
void Demo::replay() {
	 running = true;
}

/*==============================================================================
                             FUNCTION replay()
                            
   Returns true if the demonstration is running or false, otherwise.
==============================================================================*/
bool Demo::isRunning() {
	 return running;
}

/*==============================================================================
                             FUNCTION play()
                            
   Resumes demonstration from the current state.           
==============================================================================*/
void Demo::play() throw( Exception ) {
     
    if( !running ) {
        return;
    }
     
    static int clock = 0;
    static int cellNumber = 0;
     
    controller->display->zAngle += 0.5;
	if( controller->display->zAngle >= 360 )
		controller->display->zAngle = 0;

	// Changes the current state of the demonstration.		
    clock++;
	if( (clock % 150) == 0 )
		currentState++;	

    try {
    	switch( currentState) {
    	 	// Starts the initial state. The  whole  grid  is  derefined  until  the
    		// refinement level one.
    	 	case 0:	{
    		 	 while( controller->grid->getNumberOfCells() > 8 ) {
    		 	 	controller->grid->derefineAll();
    		     }
    		     currentState++;
    		}
    	 	
    		// Erases all drawings and draws the grid.
    		case 1:	{
                controller->drawGridDomain   = false;
    			controller->drawHilbertCurve = false;
    			controller->drawGraph        = false;
    			controller->drawGrid         = true;
    			break;
    		}
    
    		// Draws the Hilbert's curve
    		case 2:	{
    			controller->drawHilbertCurve = true;
    			break;
    		}
    
    		// Erases all drawings and draws the ALG's graph.
    		case 3:	{
    			controller->drawGrid         = false;
    			controller->drawHilbertCurve = false;
    			controller->drawGraph        = true;
    			break;
    		}
    
    		// Draws the Hilbert's curve.
    		case 4:	{
    			controller->drawHilbertCurve = true;
    			break;
    		}
    
    		// Erases the Hilbert's curve.
    		case 5:	{
    			controller->drawHilbertCurve = false;
    			break;
    		}
    
    		// Refines grid.
    		case 6:	{
                if( cellNumber >= controller->grid->getNumberOfCells() ) {
    				cellNumber = 0;
                }
                controller->grid->refineCellAt( cellNumber );
          		controller->grid->derefineInactiveCells();
    			cellNumber += 8;
    			break;
    		}
    		
    		// Derefines grid.
    		case 7:	{
    			controller->grid->derefineFirstBunch();
    			break;
    		}
    		
    		// Draws  grid.
    		case 8:	{
    			controller->drawGrid = true;
    			break;
    		}
    		
    		// Draws grid's domain.
    		case 9: {
                 controller->reset();
                 controller->drawGridDomain = true;
                 break;
            }
    
    		// Restarts the state 1.
    		case 10:	{
    			currentState = 1;
    			break;
    		}
    
    		default:
    			break;
    	}
    }
    catch( Exception &error ) {
           error.pushIntoStackTrace( "Demo::play()" );
           throw;
    }
}
