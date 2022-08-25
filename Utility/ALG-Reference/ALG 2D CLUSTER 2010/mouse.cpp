#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <iostream>
#include "functions.h"
#include "auxiliarMain.h"
#include "Graph.h"

extern float deltaX;
extern float deltaY;

static int mouseButton;
static int mousePreviousX;
static int mousePreviousY;

extern float scale;
extern float mouseSensibility;
extern float pointSize;
extern bool zoomIn;
extern bool zoomOut;

extern bool drawHilbertCurve;
extern bool drawGrid;
extern bool drawBlackPoints;
extern bool drawWhitePoints;
extern bool drawLineOfGroups;
extern bool printSuperGroups;
extern bool printNumberOfSuperGroups;
extern bool drawBlackCurve;
extern bool writeCellsNumber;
extern bool statisticScan;

extern Graph *graph; 
extern Grid *grid;
extern Table *whiteTable;
extern Table *blackTable;                        


/*______________________________________________________________________________
________________________________________________________________________________

                                 Main menu

    Creates a menu every time that the right button of the mouse is clicked.
________________________________________________________________________________    
______________________________________________________________________________*/
void createMenu()
{
	int menu,
		gridSubMenu,
		pointsSubMenu,
		hilbertCurveSubMenu,
		screenSubMenu,
		supergroupsSubMenu,
		simulationSubMenu;

	gridSubMenu = glutCreateMenu( gridOptions );
	glutAddMenuEntry( "Draw/Erase grid \t G", 1 );
    glutAddMenuEntry( "Write/Erase cells number", 2 );

	pointsSubMenu = glutCreateMenu( pointsOptions );
	glutAddMenuEntry( "Draw/Erase black points \t B", 1 );
    glutAddMenuEntry( "Write/Erase white points \t W", 2 );
    glutAddMenuEntry( "Black points as 'x'", 3 );
    glutAddMenuEntry( "Black points as 'o'", 4 );
    glutAddMenuEntry( "Black points as '.'", 5 );    
    
	hilbertCurveSubMenu = glutCreateMenu( hilbertCurveOptions );
	glutAddMenuEntry( "Draw/Erase Hilbert's curve \t H", 1 );
	glutAddMenuEntry( "Draw/Erase Black curve \t 2", 2 );
    	
	screenSubMenu = glutCreateMenu( screenOptions );
	glutAddMenuEntry( "Full screen", 1 );
	glutAddMenuEntry( "Default screen", 2 );
	glutAddMenuEntry( "Zoom +", 3 );
	glutAddMenuEntry( "Zoom -", 4 );
    	
	simulationSubMenu = glutCreateMenu( simulationOptions );
	glutAddMenuEntry( "Restart", 1 );
	
    supergroupsSubMenu = glutCreateMenu( supergroupsOptions );
	glutAddMenuEntry( "Draw/Erase Super-Groups \t C", 1 );
	glutAddMenuEntry( "Write/Erase Super-Groups number of the cells \t N", 2 );
	glutAddMenuEntry( "Write/Erase statistic scan of Super-Groups", 3 );
	glutAddMenuEntry( "Draw/Erase line of Groups \t L", 4 );
    	
	menu = glutCreateMenu( menuOptions );
	glutAddSubMenu( "Grid", gridSubMenu );
	glutAddSubMenu( "Hilbert's Curve", hilbertCurveSubMenu );
	glutAddSubMenu( "Screen", screenSubMenu );
	glutAddSubMenu( "ALG-Cluster simulation", simulationSubMenu );
	glutAddSubMenu( "Super-Groups", supergroupsSubMenu );
	glutAddSubMenu( "Points", pointsSubMenu );
	
	glutAddMenuEntry( "Exit \t ESC", 1 );

	glutAttachMenu( GLUT_RIGHT_BUTTON );
	glutPostRedisplay();
}

/*______________________________________________________________________________
________________________________________________________________________________

                        Control through mouse menu

    Executes options selected on main menu.
________________________________________________________________________________    
______________________________________________________________________________*/
void menuOptions( int option )
{
	switch( option )
	{
		case 1:
		{
			exit( 0 );
			break;
		}
		default: break;
	}
	glutPostRedisplay();
}

/*______________________________________________________________________________
________________________________________________________________________________

                                 Grid submenu

    Executes options selected on grid submenu.
________________________________________________________________________________
______________________________________________________________________________*/
void gridOptions( int option )
{ 	
	switch( option )
	{
		// Draws/Erases the grid
		case 1:
		{
			drawGrid = !drawGrid;
			break;
		}
		
		// Writes the number of all cells  of  the  grid  according  to  the
		// Hilbert's curve.
		case 2:
		{
			writeCellsNumber = !writeCellsNumber;
			break;
		}
		
		default: break;
	}
	glutPostRedisplay();
}

/*______________________________________________________________________________
________________________________________________________________________________

                                 Points submenu

    Executes options selected on points submenu.
________________________________________________________________________________
______________________________________________________________________________*/
void pointsOptions( int option )
{ 	
	switch( option )
	{
		// Draws/Erases the black points
		case 1:
		{
			drawBlackPoints = !drawBlackPoints;
			break;
		}
     	// Draws/Erases the white points		
		case 2:
		{
			drawWhitePoints = !drawWhitePoints;
			break;
		}
		
		case 3:
		{
             blackTable->setSymbol('x');
			break;
		}
		
        case 4:
		{
             blackTable->setSymbol('o');
			break;
		}

        case 5:
		{
             blackTable->setSymbol('.');
			break;
		}		
		default: break;
	}
	glutPostRedisplay();
}




/*______________________________________________________________________________
________________________________________________________________________________

                         Hilbert's curve submenu

    Executes options selected on Hilbert's curve submenu.
________________________________________________________________________________
______________________________________________________________________________*/
void hilbertCurveOptions( int option )
{
	switch( option )
	{
		// Draws/Erases the Hilbert's Curve.
		case 1:
		{
			drawHilbertCurve = !drawHilbertCurve;
			break;
		}
		// Draws/Erases the Black Curve.
		case 2:
        {
 	         drawBlackCurve = !drawBlackCurve;
             break;
        }
		
		default: break;
	}
	glutPostRedisplay();
}

/*______________________________________________________________________________
________________________________________________________________________________

                           Simulation submenu

    Executes options selected on simulation submenu.
________________________________________________________________________________
______________________________________________________________________________*/
void simulationOptions( int option )
{
	switch( option )
	{
		// Restarts the simulation.
		case 1:
		{
            delete graph; 
            delete grid;
            delete whiteTable;
            delete blackTable;                        
			simulation();
			break;
		}


		default: break;
	}
	glutPostRedisplay();
}

/*________________________________________________________________________________

                                Super-Groups submenu

    Executes options selected on super-groups submenu.
________________________________________________________________________________
______________________________________________________________________________*/
void supergroupsOptions( int option )
{
	switch( option )
	{
		case 1:
		{
            printSuperGroups = !printSuperGroups;
			break;
		}
		
		case 2:
		{
            printNumberOfSuperGroups = !printNumberOfSuperGroups;
			break;
		}

		case 3:
		{
            statisticScan = !statisticScan;
			break;
		}
		
		case 4:
    	{
            drawLineOfGroups = !drawLineOfGroups;
			break;
		}		

		default: break;
	}
	glutPostRedisplay();
}


/*______________________________________________________________________________
________________________________________________________________________________

                                Screen submenu

    Executes options selected on screen submenu.
________________________________________________________________________________
______________________________________________________________________________*/
void screenOptions( int option )
{
	switch( option )
	{
		// Presents the scene in full screen.
		case 1:
		{
			glutFullScreen();
			break;
		}
		
		// Presents the scene in default screen.
		case 2:
		{
			glutReshapeWindow( 700, 700 );
			glutPositionWindow( 10, 10 );
			break;
		}
		
		// Zoom +.
		case 3:
		{
             zoomIn = true;
             zoomOut = ~zoomIn;
			 scale *= 1.1;
			 mouseSensibility *= 0.95;
		 	 pointSize /= 0.95;
			 break;
		}
		
		// Zoom -.
		case 4:
		{
             zoomOut = true;
             zoomIn = ~zoomOut;
		     scale /= 1.1;
		     mouseSensibility /= 0.95;
		  	 pointSize *= 0.95;
			 break;
		}
		default: break;
	}
	glutPostRedisplay();
}


/*______________________________________________________________________________
________________________________________________________________________________

                      Control through the mouse motion.
________________________________________________________________________________
______________________________________________________________________________*/
void movingMouse( int x, int y )
{
	if( mouseButton == GLUT_LEFT_BUTTON )
	{
		deltaX += (x - mousePreviousX) * mouseSensibility;
		deltaY -= (y - mousePreviousY) * mouseSensibility;
	}
	
	mousePreviousX = x;
	mousePreviousY = y;
	glutPostRedisplay();
}

/*______________________________________________________________________________
________________________________________________________________________________

                       Control through the mouse button.
________________________________________________________________________________
______________________________________________________________________________*/
void mouse( int button, int state, int x, int y )
{
	mouseButton = button;
	
	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			break;
		
		case GLUT_MIDDLE_BUTTON:
			break;
		
		case GLUT_RIGHT_BUTTON:
          	createMenu();
			break;
	}
	
	mousePreviousX = x;
	mousePreviousY = y;
	glutPostRedisplay();
}
