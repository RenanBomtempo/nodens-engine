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
    void drawString(const char* string, void* fonte, double, double, double);  
 
/* _____________________________________________________________________________
								refinement.cpp 
______________________________________________________________________________*/

    void refineGrid( Grid *, int, double );
	void refineCell( Cell * );
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
								faces.cpp 
______________________________________________________________________________*/
    void setGridFaces( Grid * );
    struct TipoResultadoFaces visitaVizinhosCalculaSubFaces( Cell *, Cell * );
    void setCellFaceVariable( Cell *, Cell *, char );

/* _____________________________________________________________________________
								render.cpp 
______________________________________________________________________________*/
    void desenharMalha( Grid * );
    void desenharDensidade(Grid *);
    void desenharTemperatura(Grid *);
    void desenharVelocidade( Grid * );
    void desenharPressao(Grid *);
    void desenharTempo(Grid *, double, double, double );
    void desenharPausa(Grid *, double, double, double );
    void desenharValores( Grid *, int);
    void desenharLegenda( Grid *, double, double, double, int );
    void desenharPlacaPlana(Grid *);

/* _____________________________________________________________________________
								solve.cpp 
______________________________________________________________________________*/    
    void solveSuperSonic( Grid * );
    void initialCondition( Grid * );

    void setSouthBoundaryCellVariables( Cell * );
    void setNorthBoundaryCellVariables( Cell * );
    void setEastBoundaryCellVariables( Cell * );
    void setWestBoundaryCellVariables( Cell * );

    bool solutionHasConverged( Grid *, double );

/* _____________________________________________________________________________
								finitedifference.cpp 
______________________________________________________________________________*/
    double faceTauXX( Cell * , char );
    double faceTauXY( Cell * , char );
    double faceTauYY( Cell * , char );
        
    double qX( Cell * , char );
    double qY( Cell * , char );    

/* _____________________________________________________________________________
						directionalGridVariables.cpp 
______________________________________________________________________________*/
    void setGridVariables( Grid * );
    void setCellVariables( Grid *, Cell *, Cell *, char );
    void setCellBoundaryVariables( Grid *, Cell *, Cell *, char );
    
#endif
