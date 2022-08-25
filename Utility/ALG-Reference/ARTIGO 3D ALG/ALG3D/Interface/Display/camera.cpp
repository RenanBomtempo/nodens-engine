#include "camera.h"
#include <cmath>
#define rad 3.141592653589/180

/*==============================================================================
                                 Constructor
==============================================================================*/
Camera::Camera( double eye[], double center[],  double up[] ) {
	for( int i = 0; i < 3; i++ ) {
		 this->eye[i]    = eye[i];
    	 this->center[i] = center[i];
    	 this->up[i]     = up[i];
 	}
 	stepSize       = 0;
	leftRightAngle = 0;
	upDownAngle    = 0;
}

/*==============================================================================
                                  Destructor
==============================================================================*/
Camera::~Camera() {
    delete [] eye;
    delete [] center;
    delete [] up;
}

/*==============================================================================
                                getEyeVector()
==============================================================================*/
double *Camera::getEyeVector() {
	   return eye;
}

/*==============================================================================
                               getCenterVector()
==============================================================================*/
double *Camera::getCenterVector() {
	   return center;
}

/*==============================================================================
                                getUpVector()
==============================================================================*/
double *Camera::getUpVector() {
	   return up;
}

/*==============================================================================
                                setStepSize()
==============================================================================*/
void Camera::setStepSize( double value ) {
	 stepSize = value;
}

/*==============================================================================
                               setUpdownAngle()
==============================================================================*/
void Camera::setUpdownAngle( double value ) {
	 upDownAngle = value;
}

/*==============================================================================
                              setLeftRightAngle()
==============================================================================*/
void Camera::setLeftRightAngle( double value ) {
	 leftRightAngle = value;
}

/*==============================================================================
                                    walk()
==============================================================================*/
void Camera::walk( char direction ) {

	switch( direction ) {
	    case 'f':{ stepSize = fabs(stepSize);  break; } // Front.
		case 'b':{ stepSize = -fabs(stepSize); break; } // Back.
		default: { return; }
	}
	double target[3];
	for( int i = 0; i < 3; i++ ) {
		 target[i] = center[i] - eye[i];
 	}

	float targetSize = sqrt( target[0]*target[0] + target[1]*target[1] + target[2]*target[2] );

	double unitaryTarget[3];
	for( int i = 0; i < 3; i++ ) {
		 unitaryTarget[i] = target[i] / targetSize;
 	}

	for( int i = 0; i < 3; i++ ) {
		 center[i] = center[i] + ( stepSize * unitaryTarget[i] );
		 eye[i]    = eye[i] + ( stepSize * unitaryTarget[i] );
 	}
}

/*==============================================================================
                                turnAround()
==============================================================================*/
void Camera::turnAround( char direction ) {
	switch( direction ) {
	    case 'l':{ leftRightAngle -= 1; break; } // Left.
		case 'r':{ leftRightAngle += 1;	break; } // Right.
		case 'u':{ upDownAngle    += 1; break; } // Up.
		case 'd':{ upDownAngle    -= 1; break; } // Down.
		default: { return; }
	}

	double target[3];
	for( int i = 0; i < 3; i++ ) {
		 target[i] = center[i] - eye[i];
 	}

	float targetSize = sqrt( target[0]*target[0] + target[1]*target[1] + target[2]*target[2] );

	target[0] = targetSize * cos(rad * upDownAngle) * sin(rad * leftRightAngle);
	target[1] = targetSize * cos(rad * upDownAngle) * cos(rad * leftRightAngle);
	target[2] = targetSize * sin(rad * upDownAngle);

	for( int i = 0; i < 3; i++ ) {
		center[i] = target[i] + eye[i];
	}
}
