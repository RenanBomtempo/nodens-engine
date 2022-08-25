//functions.h

    
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Cell.h"
#include "Grid.h"

/* _____________________________________________________________________________
								graphics.cpp 
______________________________________________________________________________*/
    
    void setColor( int );    
    void drawLine( double, double, double, double );
    void paintCell( double, double, double );
    void paintRectangle ( double, double, double, double );
    
/* _____________________________________________________________________________
								refinement.cpp 
______________________________________________________________________________*/

    void refineGrid( Grid *, int, double, bool );
	void refineCell( Cell *, bool );
    int computeHilbertShapeNumber( int, int );
	void simplify( Cell * );
	void drawHilbertCurveLocally( Cell *, int, bool );
    
/* _____________________________________________________________________________
								derefinement.cpp 
______________________________________________________________________________*/
	
    void derefineGrid( Grid *, double, bool );
    void derefineBunch( Cell *, bool );
    bool needsDerefinement( Cell *, double );
	
/* _____________________________________________________________________________
								flux.cpp 
______________________________________________________________________________*/
    void setGridFlux( Grid * );
    void setCellFlux( Cell *, Cell *, char );
    
/* _____________________________________________________________________________
								initialCondition.cpp 
______________________________________________________________________________*/
    void initialCondition( Grid * );
    
/* _____________________________________________________________________________
								characteristicMethod.cpp 
______________________________________________________________________________*/
    void characteristicsMethod( Grid * );
    void findCharacteristicFoot( Grid *, Cell *, double[] );
    void findsCharacteristicNeighborCell( Cell *, Cell * );
    void storePreviousValues( Grid * );
    
#endif
