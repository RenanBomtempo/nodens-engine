//functions.h
#include "table.h"
#include "Grid.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define BLACK      0
#define RED        1
#define YELLOW     2
#define GREEN      3    //transparente
#define LIGHT_BLUE 4
#define BLUE       5
#define VIOLET     6
#define WHITE      7
#define ORANGE     8

#define FACTOR 150

/* _____________________________________________________________________________
								graphics.cpp 
______________________________________________________________________________*/

    void setColor( int );    
    void setLineWidth(float);
    void drawPoint( double, double, double );
    void drawLine( double, double, double, double, double, double );
    void drawRectangle ( double, double, double, double );
    void printRectangle( double , double , double , double );
    void drawString( char* , void* , double, double);  
 
/* _____________________________________________________________________________
								refinement.cpp 
______________________________________________________________________________*/
    void refineGrid( Grid *, Table *, int, int, int );
    void refineCell( Cell *, Table * );
    int computeHilbertShapeNumber( int, int );
	void simplify( Cell * );
    double dist( Cell* gridCell, Table* blackTable);


/* _____________________________________________________________________________
								keyboard.cpp 
______________________________________________________________________________*/
    void keyboard( unsigned char, int, int );

/* _____________________________________________________________________________
								  mouse.cpp 
______________________________________________________________________________*/
    void createMenu();
    void menuOptions( int option );
    void gridOptions( int option );
    void pointsOptions( int option );
    void hilbertCurveOptions( int option );
    void simulationOptions( int option );
    void supergroupsOptions( int option );
    void screenOptions( int option );
    void movingMouse( int, int );
    void mouse( int, int, int, int );


/* _____________________________________________________________________________
							initialCondition.cpp 
______________________________________________________________________________*/
void initialCondition( Grid *grid, Table *table );

#endif
