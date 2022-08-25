#ifndef CELL_H
#define CELL_H

class Cell
{
    public:
        
        Cell();			 //constructor
        ~Cell();		 //destructor

        char type;       /* 'b' = black (cell node)
		                    'w' = white (transition cell node) */
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

        int gridPosition;   /* Indicates position of cell on grid according to
		                       ordering provided by the modified Hilbert curve. */ 
        int hilbertShapeNumber; /* Variable used to compute the form of the 
		                           Hilbert curve in the bunch created when this  
								   cell is refined. */
        /* Cell geometry */
        double centerX;
        double centerY;
        double faceLength;
        double halfFaceLength;
		double xBlack;
		double yBlack;
		
		/* Variables used in counting the number of points of a cell. */
		int uBlack;
		int uWhite;		
		
        /* Variable used to match points, making groups of closer points */
		int group;
		/* Variable used to match groups */
		int superGroup;
		
	    /* Functions related to printing and drawing. */
	    void drawCell();
		void print();
		void printTransitionNode();
		void printVariable();
		void printBunch();

};//end class Cell

#endif
