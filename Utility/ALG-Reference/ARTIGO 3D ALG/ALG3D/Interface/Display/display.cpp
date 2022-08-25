#include "display.h"
#include "../controller.h"

#include "../../System/opengl.h"
#include "../../System/gl2ps.h"

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <direct.h>

// Fonts.
#define helvetica_10   GLUT_BITMAP_HELVETICA_10
#define helvetica_12   GLUT_BITMAP_HELVETICA_12
#define helvetica_18   GLUT_BITMAP_HELVETICA_18
#define times_roman_10 GLUT_BITMAP_TIMES_ROMAN_10
#define times_roman_24 GLUT_BITMAP_TIMES_ROMAN_24
#define by_13          GLUT_BITMAP_8_BY_13
#define by_15          GLUT_BITMAP_9_BY_15

// Colors.
#define BLACK      0
#define RED        1
#define YELLOW     2
#define GREEN      3
#define LIGHT_BLUE 4
#define BLUE       5
#define VIOLET     6
#define WHITE      7
#define ORANGE     8
#define GRAY       9

#define WIN_WIDTH  1000
#define WIN_HEIGHT 700;

using std::cout;
using std::cerr;
using std::endl;
using std::flush;

/*==============================================================================
                       Initialization of static members
==============================================================================*/
Display *Display::displayInstance = 0;

Camera **Display::cameras      = 0;
Camera *Display::currentCamera = 0;
int Display::numberOfCameras   = 1;

Controller *Display::controller = 0;

double Display::visionAngle = 45;
double Display::aspect      = WIN_WIDTH / WIN_HEIGHT;
double Display::Near        = 0.1;
double Display::Far         = 1000;

float Display::xAngle = 0;
float Display::yAngle = 21;
float Display::zAngle = -28;

unsigned int Display::clockTime = 8;

/*==============================================================================
                           FUNCTION createDisplay()
==============================================================================*/
Display *Display::createDisplay( Controller *controller ) {
    if( displayInstance == 0 ) {
		displayInstance             = new Display;
		displayInstance->controller = controller;
		controller->display         = displayInstance;

		/*____________________________________________________________________*
		 *                         Creation of cameras.                       *
		 *		                                                              *
		 * At the moment there is just one camera in the scene, but more than *
		 * one can easily work together.                                      *
		 *____________________________________________________________________*/
		cameras = (Camera**)malloc(numberOfCameras * sizeof(Camera *));
		currentCamera    = cameras[0];
		double eye[3]    = {3.0, 0.5, 0.5 };
		double center[3] = {0.5, 0.5, 0.5 };
		double up[3]     = {0.0, 0.0, 0.1 };
		currentCamera    = new Camera( eye, center, up );
		currentCamera->setStepSize( 0.04 );
		currentCamera->setLeftRightAngle( -90 );
		currentCamera->setUpdownAngle( 1 );

		/*____________________________________________________________________*
		 *                      Initialization of ligthts.                    *
		 *____________________________________________________________________*/
		glClearColor( 1.0, 1.0, 1.0, 0 ); //Sets background color to white.
		glShadeModel( GL_SMOOTH );

		//Define Ambient light.
		float light_ambient[4]   = { 0.2, 0.2, 0.2, 1.0 };
		float light_diffuse[4]   = { 1.0, 1.0, 1.0, 1.0 };
		float light_specular[4]  = { 1.0, 1.0, 1.0, 1.0 };
		float light_position[4]  = { 10.0, 50.0, 40.0, 1.0 };

		// Shininess
		float specularity[4] = { 1.0, 1.0, 1.0, 1.0 };
		int shininess = 80;

		// Enables lighting.
		glEnable( GL_LIGHTING );

		// Enables the light ambient.
		glLightModelfv( GL_LIGHT_MODEL_AMBIENT, light_ambient );

		// Sets the light0 parameters.
		glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient );
		glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse );
		glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular );
		glLightfv( GL_LIGHT0, GL_POSITION, light_position );

		// Enables the definition of the material color from the current color.
		glEnable( GL_COLOR_MATERIAL );

		// Define how a material reflexes light.
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specularity );
		glMateriali( GL_FRONT_AND_BACK, GL_SHININESS, shininess );

		// Turns on the light0.
		glEnable( GL_LIGHT0 );

		// Enables the depth-buffering.
		glEnable( GL_DEPTH_TEST );
	}
    return displayInstance;
}

/*==============================================================================
                              FUNCTION view()
==============================================================================*/
void Display::view() throw( Exception ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    try {
        // Makes a demonstration of the data structure.
   		controller->demo->play();

    	// Draws hilbert's curve.
    	if( controller->drawHilbertCurve )
    		controller->grid->drawHilbertCurve( RED );

    	// Draws grid.
    	if( controller->drawGrid )
    		controller->grid->drawGrid( BLACK );

    	// Draws graph.
    	if( controller->drawGraph )
    		controller->grid->drawGraph( BLUE, YELLOW, BLACK, BLUE );

    	// Draws domain.
    	if( controller->drawGridDomain )
    		controller->grid->drawDomain( BLUE );

    	// Writes the number of each cell of grid.
    	if( controller->writeCellsNumber )
    		controller->grid->drawCellsNumber( BLUE, helvetica_18 );

    	// Writes the direction of each transition node of grid.
    	if( controller->writeDirections )
    		controller->grid->drawTransitionNodesDirection( RED, by_13 );
    }
    catch( Exception &error ) {
           error.pushIntoStackTrace( "Display::view()" );
           throw;
    }

	// Renderizes the scene.
	render();
	glutSwapBuffers();
}

/*==============================================================================
                               FUNCTION render()
==============================================================================*/
void Display::render() {
	// Specifies the projection coordinates system.
	glMatrixMode( GL_PROJECTION );

	// Initializes the projection coordinates system.
	glLoadIdentity();

	// Specifies the perspective system.
	gluPerspective( visionAngle, aspect, Near, Far );

	// Specifies the model coordinates system.
	glMatrixMode( GL_MODELVIEW );

	// Initializes the model coordinates system.
	glLoadIdentity();

	// Specifies the camera's position.
	double *eye    = currentCamera->getEyeVector();
	double *center = currentCamera->getCenterVector();
	double *up     = currentCamera->getUpVector();
	gluLookAt( eye[0], eye[1], eye[2],
	           center[0], center[1], center[2],
			   up[0], up[1], up[2]
			 );

	// Rotates the drawing around the center of the grid.
	glTranslatef( 0.5, 0.5, 0.5 );
	glRotatef( xAngle, 1, 0, 0 );
	glRotatef( yAngle, 0, 1, 0 );
	glRotatef( zAngle, 0, 0, 1 );
	glTranslatef( -0.5, -0.5, -0.5 );
}

/*==============================================================================
                           FUNCTION reshapeWindow()
==============================================================================*/
void Display::reshapeWindow( int width, int height ) {
   	// Only to avoid division by zero.
	if ( height == 0 ) {
	   height = 1;
 	}
	glViewport( 0, 0, width, height );    // Specifies the viewport size.
	aspect = (float)width /(float)height; // Corrects the aspect of the window.
	render();
}

/*==============================================================================
                              FUNCTION timer()
==============================================================================*/
void Display::timer( int miliseconds ) {
    view();
    glutTimerFunc( clockTime, timer, 0 );
}


/*==============================================================================
                             FUNCTION zoonOut()
==============================================================================*/
void Display::zoonOut() {
	if( visionAngle < 130 )	{ // max vision angle = 130.
		visionAngle += 2;
	}
}

/*==============================================================================
                             FUNCTION zoonIn()
==============================================================================*/
void Display::zoonIn() {
	if( visionAngle > 0.01 ) { // minimum vision angle = 0.01.
		visionAngle -= 2;
	}
}


/*==============================================================================
                         FUNCTION getNumberOfCameras()
==============================================================================*/
int Display::getNumberOfCameras() {
    return numberOfCameras;
}

/*==============================================================================
                            FUNCTION moveCamera()
==============================================================================*/
void Display::moveCamera( char direction ) {
	 currentCamera->walk( direction );
}

/*==============================================================================
                            FUNCTION rotateCamera()
==============================================================================*/
void Display::rotateCamera( char direction ) {
	currentCamera->turnAround( direction );
}

/*==============================================================================
                          FUNCTION setCurrentCamera()
==============================================================================*/
void Display::setCurrentCamera( int cameraNumber ) {
	 if( (cameraNumber >= 0) && (cameraNumber < numberOfCameras) ) {
	 	 currentCamera = cameras[cameraNumber];
	 }
}

/*==============================================================================
                          FUNCTION printScreenshot()

    Prints a screenshot of the current screen on different file format. The file
    format is given as argument, and the format accept are: PDF, EPS, EP, SVG
    and TEX. All files are created in a directory named IMG.
==============================================================================*/
void Display::printScreenShot( string fileFormat ) {

    // Identifing the file format.
    const char *extension;
    int format;
    if( (fileFormat == "pdf") || (fileFormat == "PDF") ) {
        extension = "pdf";
        format = GL2PS_PDF;
    }
    else if( (fileFormat == "eps") || (fileFormat == "EPS") ) {
        extension = "eps";
        format = GL2PS_EPS;
    }
    else if( (fileFormat == "ps") || (fileFormat == "PS") ) {
        extension = "ps";
        format = GL2PS_PS;
    }
    else if( (fileFormat == "svg") || (fileFormat == "SVG") ) {
        extension = "svg";
        format = GL2PS_SVG;
    }
    else if( (fileFormat == "tex") || (fileFormat == "TEX") ) {
        extension = "tex";
        format = GL2PS_TEX;
    }
    else {
         cerr << "Error: Its not possible print a screenshot on a file with \
                 the extension " + fileFormat << endl;
         return;
    }

    // Creates the output file into the directory IMG.
    char folderName[20];
    char fileName[100];
    strcpy(folderName, "..\\IMG\\");

    // If the directory IMG does not exist, it is created.
    if( _mkdir( folderName ) != 0 && errno != EEXIST ) {
        cerr << "Could not create directory " << folderName << " to store image files.\n"
             << "Error: " << strerror(errno) << endl;
        return;
    }
    strcpy(fileName, folderName);
    strcat(fileName, "alg.");
    strcat(fileName, extension);

    FILE *file = fopen( fileName, "wb" );
    if( !file ) {
        cerr << "Error: The file " << fileName << " could not be opened." << endl;
        return;
    }
    cout << "Saving image to file " << fileName << " ... ";

    int state = GL2PS_OVERFLOW;
    int buffsize = 0;

    // Specifies the viewport that will be printed.
    int viewport[4];
    viewport[0] = 0;
    viewport[1] = 0;
    viewport[2] = WIN_WIDTH;
    viewport[3] = WIN_HEIGHT

    // Prints the current screenshot.
    int options = GL2PS_DRAW_BACKGROUND |
                  GL2PS_OCCLUSION_CULL |
                  GL2PS_BEST_ROOT |
                  GL2PS_SIMPLE_LINE_OFFSET;
    while(state == GL2PS_OVERFLOW){
        buffsize += 1024*1024;
        gl2psBeginPage("ALG-3D Adaptive Mesh Refinement", // Title
                       "ALG-3D",                          // Producer
                       viewport,                          // Viewport
                       format,                            // File format
                       GL2PS_BSP_SORT,                    // Sorting algorithm
                       options,                           // Global options
                       GL_RGBA,                           // Color mode
                       0,                                 // Size of color map
                       NULL,                              // Color map
                       0,                                 // Number of flat shaded ( red )
                       0,                                 // Number of flat shaded ( green )
                       0,                                 // Number of flat shaded ( blue )
                       buffsize,                          // Size of the feedback buffer
                       file,                              // Stream to which data is printed
                       fileName                           // Name of the stream to which data is printed
                       );

        this->view();
        state = gl2psEndPage();
    }
    fclose(file);
    cout << "Done!\n" << flush;
}
