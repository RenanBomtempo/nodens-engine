#ifndef CAMERA_H
#define CAMERA_H

/*==============================================================================
                                  Camera
==============================================================================*/
class Camera {
	private:
		double eye[3], center[3], up[3];
		double stepSize, upDownAngle, leftRightAngle;
		
	public:
	    Camera( double eye[], double center[], double up[] );
		~Camera();
		double* getEyeVector();
		double* getCenterVector();
		double* getUpVector();
		void setStepSize( double value );
		void setUpdownAngle( double value );
		void setLeftRightAngle( double value );
		void walk( char direction );
		void turnAround( char direction );
};

#endif

	
