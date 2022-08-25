#ifndef MOUSE_H
#define MOUSE_H

#include "../controller.h"

/*==============================================================================
                                     Mouse
                                    
    This class is singleton. It means that it is impossible to create more than
    one object of this class.
==============================================================================*/
class Mouse {
private:
	static Mouse *mouseInstance;
	Mouse(){}
	Mouse(Mouse &){}
	Mouse& operator=(Mouse &);
	
	static Controller *controller;
	
	static float previousX;
	static float previousY;
	static int   mouseButton;

	static void menuOptions( int );
	static void gridOptions( int ) throw (Exception);;
	static void graphOptions( int );
	static void hilbertCurveOptions( int );
	static void demoOptions( int ) throw (Exception);;
	static void domainOptions( int );
	static void setDomain( int, float, float ) throw (Exception);
	static void typeOfDomainOptions( int );
	static void screenOptions( int );
	static void createMenu( void ) throw (Exception);
	
public:
    static Mouse *createMouse( Controller *controller );
    static void execute( int, int, int, int );
    static void moveMouse( int, int );
};

#endif
