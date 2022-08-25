//functions.h
  
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Cell.h"
#include "Grid.h"
#include "stdio.h"

/* _____________________________________________________________________________
								graphics.cpp 
______________________________________________________________________________*/

    void setColor( int );    
    void drawLine( double, double, double, double, double, double );
    void paintCell( double, double, double );
    void paintRectangle ( double, double, double, double );
    void drawString(const char* string, void* fonte, double, double, double);  
 
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
    double computeExpressionFlux( Cell *, Cell *, int , char );

/* _____________________________________________________________________________
								render.cpp 
______________________________________________________________________________*/
    void desenharMalha( Grid * );
    void desenharPlacaPlana(Grid *);
    void desenharValores( Grid *, int);
    void desenharTempo(Grid *, double);
    void desenharDensidade(Grid *);
    void desenharTemperatura(Grid *);
    void desenharPressao(Grid *);
    void desenharPausa(Grid *, double );
    void desenhaVetor( double, double, double , double , double, double );
    void desenhaCampoVetorial( Grid * );

/* _____________________________________________________________________________
								solve.cpp 
______________________________________________________________________________*/    
    void solveSuperSonic( Grid * );
    void initialCondition( Grid * );

    void setSouthBoundaryCellVariables( Grid * , Cell * );
    void setNorthBoundaryCellVariables( Cell * );
    void setEastBoundaryCellVariables( Cell * );
    void setWestBoundaryCellVariables( Cell * );

    bool solutionHasConverged( Grid *, double );

/* _____________________________________________________________________________
								finitedifference.cpp 
______________________________________________________________________________*/
    double tauXX( Cell * , char );
    double tauXY( Cell * , char );
    double tauYY( Cell * , char );
        
    double qX( Cell * , char );
    double qY( Cell * , char );    

/* _____________________________________________________________________________
						directionalGridVariables.cpp 
______________________________________________________________________________*/
    void setGridVariables( Grid * );
    void setCellVariables( Grid *, Cell *, Cell *, char );
    void setCellBoundaryVariables( Grid *, Cell *, Cell *, char );
    
#endif
