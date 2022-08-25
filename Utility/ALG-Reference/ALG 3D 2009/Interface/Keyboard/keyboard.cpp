#include <iostream>

#include "../../System/opengl.h"
#include "keyboard.h"

using namespace std;

static float topCut = 1;

void setDomain( Grid *, int, float, float );

/*==============================================================================
                       Initilization of static members
==============================================================================*/
Keyboard   *Keyboard::keyboardInstance = 0;
Controller *Keyboard::controller       = 0;

/*==============================================================================
                            FUNCTION createKeyboard()

    It creates only one object of the class Keyboard. If this function is called
    more than once, the first object created is returned.
==============================================================================*/
Keyboard* Keyboard::createKeyboard( Controller *controller ) {
	if( keyboardInstance == 0 ) {
		keyboardInstance = new Keyboard;
		keyboardInstance->controller = controller;
	}
    return keyboardInstance;
}

/*==============================================================================
                                FUNCTION execute()
==============================================================================*/
void Keyboard::execute( unsigned char key, int x, int y ) {
	switch( key ) {
		// Amplification
		case '=':
		case '+':
			controller->display->zoonIn();
			break;

		// shrink
		case '-':
		case '_':
			controller->display->zoonOut();
			break;

		// Draws grid, if it is not drawn, or erases it if it has been painted.		case 'g':
		case 'g':
		case 'G':
			controller->drawGrid = !controller->drawGrid;
			break;

		// Draws Hilbert's curve, if it is not drawn, or erases it if it has
		// been painted.
		case 'h':
		case 'H':
			controller->drawHilbertCurve = !controller->drawHilbertCurve;
			break;

		// Draws graph if it is not drawn or erases it if it has been painted.
		case 'b':
		case 'B':
			controller->drawGraph = !controller->drawGraph;
			break;

		// Pauses demonstration
		case 'p':
		case 'P':
             if( controller->demo->isRunning() ) {
                 controller->demo->pause();
             }
             else {
                  controller->demo->replay();
             }
			break;

		// Restarts demonstration from the beginning.
		case 'r':
		case 'R':
			controller->demo->restart();
			break;

		// Writes all cell numbers.
		case 'n':
		case 'N':
			controller->writeCellsNumber = !controller->writeCellsNumber;
			break;

		// Writes trasition nodes's direction.
		case 'm':
		case 'M':
			controller->writeDirections= !controller->writeDirections;
			break;

		// Finishes the program when key ESC is pushed.
		case 27:
			exit(0);
			break;

		default:
			break;
	}
	glutPostRedisplay();
}

#include <iostream>

/*==============================================================================
                 Control through special keys of the keyboard
==============================================================================*/
void Keyboard::executeSpecialKey( int key, int x, int y ) throw (Exception) {
	static int cellNumber = 0;

	try {
        switch( key ) {
    		// Goes ahead in the direction pointed by the current camera.
    		case GLUT_KEY_UP:
    		 	controller->display->moveCamera('f');
    		 	break;

    	    // Comes back from the direction pointed by the current camera.
    		case GLUT_KEY_DOWN:
    			controller->display->moveCamera('b');
    			break;

    		// Turns the current camera to its left side.
    		case GLUT_KEY_LEFT:
    			controller->display->rotateCamera('l');
    			break;

    		// Turns the current camera to its right side.
    		case GLUT_KEY_RIGHT:
    			controller->display->rotateCamera('r');
    			break;

    		// Turns the current camera upward.
    		case GLUT_KEY_PAGE_UP:
    			controller->display->rotateCamera('u');
    			break;

    		// Turns the current camera downward.
    		case GLUT_KEY_PAGE_DOWN:
    			controller->display->rotateCamera('d');
    			break;

    		// Full screen mode.
    		case GLUT_KEY_F1:
    			glutFullScreen();
    			break;

    		// Default screen mode.
    		case GLUT_KEY_F2:
    			glutReshapeWindow( 1000, 700 );
    			glutPositionWindow( 10, 30 );
    			break;

    		// Erases or draw domain.
    		case GLUT_KEY_F3:
    			controller->drawGridDomain = !controller->drawGridDomain;
    			break;

    		// Refines a cell.
    	 	case GLUT_KEY_F5:
                if( cellNumber >= controller->grid->getNumberOfCells() ) {
    				cellNumber = 0;

    			}
    			controller->grid->refineCellAt( cellNumber );
    			setDomain( controller->grid,
    					   controller->domainNumber,
    					   controller->domainParameter1,
    					   controller->domainParameter2
    					  );
    			controller->grid->derefineInactiveCells();
    			cellNumber += 8;
    			break;

    		// Derefines first bunch of a grid
    		case GLUT_KEY_F6:
    			controller->grid->derefineFirstBunch();
    			cellNumber = 0;
    			break;

    		// Refines whole grid
    		case GLUT_KEY_F7:
    			controller->grid->refineAll();
    			setDomain( controller->grid,
    					   controller->domainNumber,
    					   controller->domainParameter1,
    					   controller->domainParameter2
    					  );
    			controller->grid->derefineInactiveCells();
    			break;

    		// Derefines whole grid
    		case GLUT_KEY_F8:
    			controller->grid->derefineAll();
    			setDomain( controller->grid,
    					   controller->domainNumber,
    					   controller->domainParameter1,
    					   controller->domainParameter2
    					  );
    			cellNumber = 0;
    			break;

    		// Prints a screenshot of current screen on different formats. All
    		// file are placed in the directory IMG.
    		case GLUT_KEY_F12:
                 controller->display->printScreenShot("pdf");
                 controller->display->printScreenShot("eps");
                 controller->display->printScreenShot("ps");
                 break;

            case GLUT_KEY_HOME:
                 if( topCut < 1 ) topCut += 0.05;
                 else topCut = 1;

                 std::cout << topCut << std::endl;

                 controller->grid->setTopPlane( topCut );
                 break;

            case GLUT_KEY_END:
                 if( topCut > 0 ) topCut -= 0.05;
                 else topCut = 0;

                 std::cout << topCut << std::endl;

                 controller->grid->setTopPlane( topCut );
                 break;

    		default:
    		    break;
    	}
    }
    catch( Exception &error ) {
           error.pushIntoStackTrace("Keyboard::executeSpecialKey");
           throw;
    }

	glutPostRedisplay();
}

/*==============================================================================
   Prints on the standard  output information about all commands of the keyboard.
==============================================================================*/
void Keyboard::printCommands() {
    cout << endl;
    cout << "KEYBOARD COMMANDS:" << endl << endl;
	cout << "F1:\t Full screen" << endl;
	cout << "F2:\t Default screen" << endl;
	cout << "F3:\t Draw domain" << endl;
	cout << "F5:\t Refine first bunch" << endl;
	cout << "F6:\t Derefine first bunch" << endl;
	cout << "F7:\t Refine whole grid uniformly" << endl;
	cout << "F8:\t Derefine whole grid uniformly" << endl;
	cout << "F12:\t Print screenshot of the current image on the following" << endl;
	cout << "\t file formats: PDF, EPS, and PS" << endl;

	cout << endl;
	cout << "+:\t Amplification" << endl;
	cout << "-:\t Shrink" << endl;
	cout << "UP:\t Move camera forward" << endl;
	cout << "DOWN:\t Move camera backward" << endl;
	cout << "LEFT:\t Move camera to the left" << endl;
	cout << "RIGHT:\t Move camera to the right" << endl;
	cout << "PgUp:\t Move camera upward" << endl;
	cout << "PgDown:\t Move camera downward" << endl;

	cout << endl;
	cout << "B:\t Show\\Hide graph" << endl;
	cout << "G:\t Show\\Hide grid" << endl;
	cout << "H:\t Show\\Hide Hilbert's curve" << endl;
	cout << "M:\t Show\\Hide transition nodes' direction name" << endl;
	cout << "N:\t Show\\Hide cell nodes' number" << endl;
	cout << "P:\t Play\\Pause demonstration" << endl;
	cout << "R:\t Restart demonstration from the beginning" << endl;

	cout << endl;
	cout << "ESQ:\t Exit" << endl;
}

void setDomain( Grid *grid, int domainNumber = 0, float parameter1 = 0, float parameter2 = 0.5 ){
	switch( domainNumber )	{
		case 0:
			grid->setCubicalDomain();
			break;

		case 1:
			grid->setSphericalDomain( parameter1, parameter2 );
			break;

		case 2:
			grid->setCylindricalDomain( parameter1, parameter2 );
			break;

		case 3:
			grid->setConicalDomain();
			break;

		case 4:
			grid->setParabolicalDomain();
			break;

		default:
			break;
	}
}
