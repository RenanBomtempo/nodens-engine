/*==============================================================================
               Three-dimensional Autonomous Leaves Graph - ALG-3D

 - Authors:
   o Denise Burgarelli Duczmall - burgarel@mat.ufmg.br
   o Maycon da Costa - maycon-@ufmg.br
   o Rodney Jozué Biezuner - rodney@mat.ufmg.br

 - Last updating: july 13th, 2009
==============================================================================*/

// Interface
#include "Mouse/mouse.h"
#include "Keyboard/keyboard.h"
#include "Display/display.h"
#include "Demo/demo.h"

// Application
#include "../ALG/Grid/grid.h"

// System
#include <iostream>
#include "../System/opengl.h"

#define WIN_WIDTH 1000
#define WIN_HEIGTH 700

using std::cout;
using std::endl;

/*==============================================================================
                                   Main()
==============================================================================*/
int main( int argc, char** argv ) {
	try {
		// Creates a window.
		glutInit( &argc, argv );
		glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE );
		glutInitWindowSize( WIN_WIDTH, WIN_HEIGTH );
		glutInitWindowPosition ( 10, 10 );
		glutCreateWindow( "Three-dimensional Autonomous Leaves Graph - ALG-3D" );

		// Creates a new grid and makes the necessary initializations.
		Grid *grid = new Grid;
		//grid->refineAll();

		/* Creates an object controller to make connections between ALG and
		 * components of the user interface. */
		Controller *controller = Controller::createController( grid );

		// Creates an object to deal with the demostration of the data structure.
		Demo *demo = Demo::createDemo( controller );
		//demo->start();

		// Creates all devices to make interface with user.
		Display  *display  = Display::createDisplay( controller );
		Mouse    *mouse    = Mouse::createMouse( controller );
		Keyboard *keyboard = Keyboard::createKeyboard( controller );

		// Starts the state machine of OpenGL.
		glutDisplayFunc ( display->view );
		glutReshapeFunc ( display->reshapeWindow );
        glutTimerFunc   ( display->clockTime, display->timer, 0 );
		glutKeyboardFunc( keyboard->execute );
		glutSpecialFunc ( keyboard->executeSpecialKey );
		glutMouseFunc   ( mouse->execute );
		glutMotionFunc  ( mouse->moveMouse );

		cout << "===============   ALG-3D   ===============" << endl;
		keyboard->printCommands();
        cout << endl;

		glutMainLoop();
	}
	catch( Exception &error ) {
		error.pushIntoStackTrace( "main()" );
		error.printStackTrace();
		exit(1);
 	}
}
