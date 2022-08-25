#ifndef DISPLAY_H
#define DISPLAY_H

#include "../../ALG/Exceptions/exceptions.h"
#include "camera.h"

#include <string>
using std::string;

class Controller;

/*==============================================================================
                                  Display
                                    
    This class is singleton. It means that it is impossible to create more than
    one object of this class.
==============================================================================*/
class Display {
private:
	
	/* Statements to make this class be singleton.*/	
	static Display *displayInstance;
	Display(){}
	Display(Display &){}
	Display& operator = (Display &);
			 
	static Controller *controller;
	
	/* Set of cameras. */
	static Camera **cameras;
	static Camera *currentCamera;
	static int numberOfCameras;
	
	/* Variables used in perspective projection */
	static double visionAngle;
	static double aspect;
	static double Near;
	static double Far;
	
	static void render();

public:
	static Display *createDisplay( Controller *controller );
	static void view() throw( Exception );
	static void reshapeWindow( int width, int height );
	static void timer( int miliseconds );
	
	// Functions to manipulate cameras.
	int  getNumberOfCameras();
	void setCurrentCamera( int cameraNumber );
	void moveCamera( char direction );
	void rotateCamera( char direction );

	void printScreenShot( string fileFormat );	
		
	void zoonIn();
	void zoonOut();
	
	static float xAngle;
	static float yAngle;
	static float zAngle;
	
	static unsigned int clockTime;
};

#endif
