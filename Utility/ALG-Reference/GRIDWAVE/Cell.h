/* Cell.h
   Definition of class Cell.
*/

#ifndef CELL_H
#define CELL_H

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
//Variables used in solving the wave equation.

        double u;         /* Approximate value of solution in this cell. */
        double previousU;/* Value of u in previous instant of time. */
        
        //Fluxes used to decide if a cell should be refined or if a bunch
        //should be derefined.
        double northFlux, // Flux coming from north direction.
               southFlux, // Flux coming from south direction.
               eastFlux,  // Flux coming from east direction.
               westFlux;  // Flux coming from west direction.

//______________________________________________________________________________
//Print operators.

		void print();
		void printTransitionNode();
		void printVariable();
		void printBunch();

};//end class Cell

#endif
