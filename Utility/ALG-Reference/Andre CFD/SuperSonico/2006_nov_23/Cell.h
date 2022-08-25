/* Cell.h
   Definition of class Cell.
*/

#ifndef CELL_H
#define CELL_H

#include "Element.h"

class Element;

class Cell
{
    public:
        
        Cell();			 //constructor
        ~Cell();		 //destructor
//______________________________________________________________________________
//Data structure.

        char type;       /* 'b' = black (cell node)
		                    'w' = white (transition cell node) */
        bool active;     // Indicates if cell is active (true) or inactive (false).
        int level;       // Refinement level (initial level = 0).
        
        //Pointers that only cell nodes use:
        int bunchNumber; /* Bunch identifier (cells derived from same father 
                                              have same bunchNumber).*/        
        Cell *north;     // Points to cell or transition node above this cell.
        Cell *south;     // Points to cell or transition node below this cell.
        Cell *east;      // Points to cell or transition node rightward of this cell.
        Cell *west;      // Points to cell or transition node leftward of this cell.

        Cell *previous;  // Previous cell of the grid in the Hilbert curve ordering.
        Cell *next;      // Next cell of the grid in the Hilbert curve ordering.

        //Pointers that only transition nodes use:
        Cell *singleConnector;
        Cell *doubleConnector1;
        Cell *doubleConnector2;

//______________________________________________________________________________
//Grid ordering through Hilbert curve.

        int gridPosition;   /* Indicates position of cell on grid according to
		                       ordering provided by the modified Hilbert curve. */ 
        int hilbertShapeNumber; /* Variable used to compute the form of the 
		                           Hilbert curve in the bunch created when this  
								   cell is refined. */

//______________________________________________________________________________
//Cell geometry.

        double centerX;
        double centerY;
        double faceLength;
        double halfFaceLength;

//______________________________________________________________________________
//Variables used in solving the Navier-Stokes equations.

    double u, previousU; 

    // Variaveis de fluido nesta celula ----------------
    double rhoAtual,
           rhoAnterior,
           rhoIterAnterior;

    double uAtual,
           uAnterior,
           uIterAnterior;

    double vAtual,
           vAnterior,
           vIterAnterior;

    double eAtual,
           eAnterior,
           eIterAnterior;
    
    double TAtual,
           TAnterior,
           TIterAnterior;

    double pAtual,
           pAnterior,
           pIterAnterior;

    double miAtual,
           miAnterior,
           miIterAnterior;

    double kAtual,
           kAnterior,
           kIterAnterior;

    // Variaveis de fluido nos vizinhos ----------------
    double northRho,
           southRho,
           eastRho,
           westRho;

    double northU,
           southU,
           eastU,
           westU;

    double northV,
           southV,
           eastV,
           westV;

    double northE,
           southE,
           eastE,
           westE;

    double northT,
           southT,
           eastT,
           westT;

    double northP,
           southP,
           eastP,
           westP;

    double northMi,
           southMi,
           eastMi,
           westMi;

    double northK,
           southK,
           eastK,
           westK;
                     
        //Fluxes used to decide if a cell should be refined or if a bunch
        //should be derefined.
        double northFlux, // Flux coming from north direction.
               southFlux, // Flux coming from south direction.
               eastFlux,  // Flux coming from east direction.
               westFlux;  // Flux coming from west direction.

//______________________________________________________________________________
/* Variables used in solving the discretized system Ax = b through the conjugate gradient method.
   The grid discretization matrix and its resolution are directly implemented on the grid, 
   which improves performance. There is no independent linear algebra package. */

        double b;   /* In Ax = b, corresponds to the element in vector b associated to this cell. */
        double Ax;  /* Element of vector Ax = b associated to this cell. Also plays the role of Ap.*/
        double r;   /* Element of the vector r = b - Ax associated to this cell. */
        double p;   /* Element of the search direction vector in the conjugate gradient algorithm. */
        double p1;  /* p's upgrade in the conjugate gradient algorithm. */

//______________________________________________________________________________

//Print operators.

		void print();
		void printTransitionNode();
		void printVariable();
		void printBunch();

        Element *firstElement;

};//end class Cell

#endif
