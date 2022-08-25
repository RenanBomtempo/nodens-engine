#include "../../System/opengl.h"
#include "mouse.h"

/*==============================================================================
                       Initialization of static members
==============================================================================*/
Mouse *Mouse::mouseInstance   = 0;
float  Mouse::previousX       = 0;
float  Mouse::previousY       = 0;
int    Mouse::mouseButton     = 0;
Controller *Mouse::controller = 0;

/*==============================================================================
                            FUNCTION createMouse()

    It creates only one object of the class Mouse. If this function is called
    more than once, the first object created is returned.
==============================================================================*/
Mouse *Mouse::createMouse( Controller *controller ) {
  	if( mouseInstance == 0 ) {
	    mouseInstance             = new Mouse;
		mouseInstance->controller = controller;
	}
    return mouseInstance;
}

/*==============================================================================
                            FUNCTION moveMouse()

    It is responsible for moving drawings on screen when the left button is
    kept pushed.
==============================================================================*/
void Mouse::moveMouse( int x, int y ) {
	const float sensibility = 0.333;
	if( mouseButton == GLUT_LEFT_BUTTON ) {
		controller->display->zAngle += (x - previousX) * sensibility;
		controller->display->yAngle += (y - previousY) * sensibility;
	}
	previousX = x;
	previousY = y;
	glutPostRedisplay();
}

/*==============================================================================
                            FUNCTION execute()

    Updates the state of the object mouse when its pointer moves around the
    screen.
==============================================================================*/
void Mouse::execute( int button, int state, int x, int y ) {
	mouseButton = button;
	switch( button ) {
		case GLUT_LEFT_BUTTON:
			break;

		case GLUT_MIDDLE_BUTTON:
			break;

		case GLUT_RIGHT_BUTTON:
			createMenu();
			break;
	}
	previousX = x;
	previousY = y;
	glutPostRedisplay();
}

/*==============================================================================
                              FUNCTION createMenu()

    Creates a menu every time that the right button of the mouse is clicked.
==============================================================================*/
void Mouse::createMenu() throw (Exception) {
	int menu,
		gridSubMenu,
		graphSubMenu,
		hilbertCurveSubMenu,
		domainSubMenu,
		typeOfDomainSubMenu,
		screenSubMenu,
		demonstrationSubMenu;

	gridSubMenu = glutCreateMenu( gridOptions );
	glutAddMenuEntry( "Draw grid \t G", 1 );
	glutAddMenuEntry( "Erase grid \t G", 2 );
	glutAddMenuEntry( "Refine grid uniformly \t F7", 3 );
	glutAddMenuEntry( "Derefine grid uniformly \t F8", 4 );
	glutAddMenuEntry( "Write transition nodes direction \t M", 5 );
	glutAddMenuEntry( "Erase transition nodes direction \t M", 6 );
	glutAddMenuEntry( "Write cells number \t N", 7 );
	glutAddMenuEntry( "Erase cells number \t N", 8 );
	glutAddMenuEntry( "Print grid in an output file", 9 );

	graphSubMenu = glutCreateMenu( graphOptions );
	glutAddMenuEntry( "Draw graph \t B", 1 );
	glutAddMenuEntry( "Erase graph \t B", 2 );

	hilbertCurveSubMenu = glutCreateMenu( hilbertCurveOptions );
	glutAddMenuEntry( "Draw Hilbert's curve \t H", 1 );
	glutAddMenuEntry( "Erase Hilbert's curve \t H", 2 );

	typeOfDomainSubMenu = glutCreateMenu( typeOfDomainOptions );
	glutAddMenuEntry( "Cube", 0 );
	glutAddMenuEntry( "Sphere", 1 );
	glutAddMenuEntry( "Cylinder", 2 );
	glutAddMenuEntry( "Cone", 3 );
	glutAddMenuEntry( "Paraboloid", 4 );

	domainSubMenu = glutCreateMenu( domainOptions );
	glutAddSubMenu( "Set domain", typeOfDomainSubMenu );
	glutAddMenuEntry( "Draw domain \t F3", 1 );
	glutAddMenuEntry( "Erase domain \t F3", 2 );

	screenSubMenu = glutCreateMenu( screenOptions );
	glutAddMenuEntry( "Full screen \t F1", 1 );
	glutAddMenuEntry( "Default screen \t F2", 2 );

	demonstrationSubMenu = glutCreateMenu( demoOptions );
	glutAddMenuEntry( "Pause \t P", 1 );
	glutAddMenuEntry( "Replay \t P", 2 );
	glutAddMenuEntry( "Restart from the beginning \t R", 3 );

	menu = glutCreateMenu( menuOptions );
	glutAddSubMenu( "Grid", gridSubMenu );
	glutAddSubMenu( "Graph", graphSubMenu );
	glutAddSubMenu( "Hilbert's Curve", hilbertCurveSubMenu );
	glutAddSubMenu( "Domain", domainSubMenu );
	glutAddSubMenu( "ALG demonstration", demonstrationSubMenu );
	glutAddSubMenu( "Screen", screenSubMenu );
	glutAddMenuEntry( "Exit \t ESC", 1 );

	glutAttachMenu( GLUT_RIGHT_BUTTON );
	glutPostRedisplay();
}

/*==============================================================================
                                Menu options

    Executes options selected on main menu.
==============================================================================*/
void Mouse::menuOptions( int option ) {
	switch( option ) {
		case 1:	{
			exit( 0 );
			break;
		}
		default: break;
	}
	glutPostRedisplay();
}

/*==============================================================================
                                 Grid submenu

    Executes options selected on grid submenu.
==============================================================================*/
void Mouse::gridOptions( int option ) throw (Exception) {
    try {
        switch( option ) {
    		// Draws grid
    		case 1:	{
    			controller->drawGrid = true;
    			break;
    		}

    		// Erases grid.
    		case 2:	{
    			controller->drawGrid = false;
    			break;
    		}

    		// Refines whole grid.
    		case 3:	{
    			controller->grid->refineAll();
    			setDomain( controller->domainNumber,
    					   controller->domainParameter1,
    					   controller->domainParameter2
       					 );
    			controller->grid->derefineInactiveCells();
    			break;
    		}

    		// Derefines whole grid.
    		case 4:	{
    			controller->grid->derefineAll();
    			setDomain( controller->domainNumber,
    					   controller->domainParameter1,
    					   controller->domainParameter2
       					 );
    			break;
    		}

    		// Writes direction of all transition node of current grid.
    		case 5:	{
    			controller->writeDirections = true;
    			break;
    		}

    		// Erases information about direction of all transition nodes.
    		case 6:	{
    			controller->writeDirections = false;
    			break;
    		}

    		// Writes the number of all cells  of  the  grid  according  to  the
    		// Hilbert's curve.
    		case 7: {
    			controller->writeCellsNumber = true;
    			break;
    		}

    		// Erases cells' number.
    		case 8:	{
    			controller->writeCellsNumber = false;
    			break;
    		}

    		// Prints grid in an output file.
    		case 9:	{
    			controller->grid->print();
    			break;
    		}

    		default: break;
    	}
    }
    catch( Exception &error ) {
           error.pushIntoStackTrace("");
           throw;
    }
	glutPostRedisplay();
}

/*==============================================================================
                             Graph submenu

    Executes options selected on graph submenu.
==============================================================================*/
void Mouse::graphOptions( int option ) {
	switch( option ) {
		// Draws graph.
		case 1:	{
			controller->drawGraph = true;
			break;
		}

		// Erases graph.
		case 2:	{
			controller->drawGraph = false;
			break;
		}

		default: break;
	}
	glutPostRedisplay();
}

/*==============================================================================
                         Hilbert's curve submenu

    Executes options selected on Hilbert's curve submenu.
==============================================================================*/
void Mouse::hilbertCurveOptions( int option ) {
	switch( option ) {
		// Draws Hilbert's Curve.
		case 1: {
			controller->drawHilbertCurve = true;
			break;
		}

		// Erases Hilbert's Curve.
		case 2: {
			controller->drawHilbertCurve = false;
			break;
		}

		default: break;
	}
	glutPostRedisplay();
}

/*==============================================================================
                           Demonstration submenu

    Executes options selected on demonstration submenu.
==============================================================================*/
void Mouse::demoOptions( int option ) throw (Exception) {
    try {
        switch( option ) {
    		// Pauses demonstration.
    		case 1:	{
    			controller->demo->pause();
    			break;
    		}

    		// Restarts demonstration.
    		case 2:	{
    			controller->demo->replay();
    			break;
    		}

    		// Restarts demonstration from the beginning.
    		case 3:	{
    			controller->demo->restart();
    			break;
    		}

    		default: break;
    	}
    }
    catch( Exception &error ) {
           error.pushIntoStackTrace("");
           throw;
    }
	glutPostRedisplay();
}

/*==============================================================================
                                Screen submenu

    Executes options selected on screen submenu.
==============================================================================*/
void Mouse::screenOptions( int option ) {
	switch( option ) {
		// Presents scene in full screen.
		case 1: {
			glutFullScreen();
			break;
		}

		// Presents scene in default screen.
		case 2: {
			glutReshapeWindow( 1000, 700 );
			glutPositionWindow( 10, 10 );
			break;
		}

		default: break;
	}
	glutPostRedisplay();
}

/*==============================================================================
                               Domain submenu

    Executes options selected on domain submenu.
==============================================================================*/
void Mouse::domainOptions( int option) {
	switch( option ) {
		case 1:
			controller->drawGridDomain = true;
			break;

		case 2:
			controller->drawGridDomain = false;
			break;

		default: break;
	}
	glutPostRedisplay();
}

/*==============================================================================
                           Type of domain submenu

    Executes options selected on type of domain submenu.
==============================================================================*/
void Mouse::typeOfDomainOptions( int option ) {
	controller->domainNumber = option;
	setDomain( controller->domainNumber,
			   controller->domainParameter1,
			   controller->domainParameter2
   			 );
	glutPostRedisplay();
}

/*==============================================================================                             FUNCTION setDomain()
                                   setDomain()

    Sets the current domain of a grid. It receives as parameters the number of
	the domain to be setted, and the parameters of the domain choosen.

	Domain Numbers:
		   0: Cube.
		   1: Sphere.
		   2: Cylinder.
		   3: Cone.
		   4: Paraboloid.
==============================================================================*/
void Mouse::setDomain( int domainNumber = 0, float parameter1 = 0, float parameter2 = 0.5 ) throw (Exception) {
    try {
        switch( domainNumber )	{
    		case 0:
    			controller->grid->setCubicalDomain();
    			break;

    		case 1:
    			controller->grid->setSphericalDomain( parameter1, parameter2 );
    			break;

    		case 2:
    			controller->grid->setCylindricalDomain( parameter1, parameter2 );
    			break;

    		case 3:
    			controller->grid->setConicalDomain();
    			break;

    		case 4:
    			controller->grid->setParabolicalDomain();
    			break;

    		default:
    			break;
    	}
    }
    catch( Exception &error ) {
           error.pushIntoStackTrace("Mouse::setDomain");
           throw;
    }
}
