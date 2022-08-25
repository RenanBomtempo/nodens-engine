#ifndef CELL_H_
#define CELL_H_

#include "../../Exceptions/invalidParameterValue.h"

/*=============================================================================*
 *                                    Cell                                     *
 *=============================================================================*/
class Cell {
	protected:
		char type;   // Cell identifier.
		bool active; // Indicates if cell is active (true) or inactive (false).
		int  level;  // Refinement level.
		
		// Cell coordinates.
		float centerX,
			  centerY,
			  centerZ;

		friend class Grid;
		friend class CellNode;
		friend class TransitionNode;
		
	public:
		Cell( const char, bool, int, float, float, float );
		virtual ~Cell();
		virtual void draw( int );
		virtual void print() = 0;
};


/*=============================================================================*
 *                                  CellNode                                  *
 *=============================================================================*/
class CellNode: public Cell {
	private:
		long int bunchNumber; // Bunch identifier
		
		Cell *north; // Points to cell node or transition node above this cell.
		Cell *south; // Points to cell node or transition node below this cell.
		Cell *east;  // Points to cell node or transition node rightward this cell.
		Cell *west;  // Points to cell node or transition node leftward this cell.
		Cell *front; // Points to cell node or transition node in front of this cell.
		Cell *back;  // Points to cell node or transition node behind this cell.
		
		CellNode *previous; // Previous cell of the grid in the Hilbert curve ordering.
		CellNode *next;     // Next cell of the grid in the Hilbert curve ordering.
		
		/* Indicates position of cell on grid according to  ordering provided by
		 * the modified Hilbert curve. */
		long int gridPosition;
		
		/* Variable used to storage the form of the  Hilbert curve in the  bunch
		 * created when this cell is refined. */
		int hilbertShapeNumber; 
		
		// Cell geometry.
		float faceLength;
		float halfFaceLength;
		
		// Variables used by some aplications of partial differential equations.
		float u;
		float previousU;
		
		// Indicates if this cell is to be drawn or not.
		bool isToPaint;
	
		// Fluxes used to decide if a cell should be refined or if a bunch
		// should be derefined.
		float northFlux, // Flux coming from north direction.
			  southFlux, // Flux coming from south direction.
			  eastFlux,  // Flux coming from east direction.
			  westFlux,  // Flux coming from west direction.
			  frontFlux, // Flux coming from front direction.
			  backFlux;  // Flux coming from back direction.

		friend class Grid;
		friend class TransitionNode;

	public:
		CellNode();
		~CellNode();	
	
		void print();
		void printVariable();
		void drawWiredCell();
		void drawSolidCell();
		void setCellFlux( char ) throw(InvalidParameterValue);
		float getMaximumFlux();
};

/*=============================================================================*
 *                                  TransitionNode                                  *
 *=============================================================================*/
class TransitionNode: public Cell  {
	private:
		Cell *singleConnector;
		Cell *quadrupleConnector1;
		Cell *quadrupleConnector2;
		Cell *quadrupleConnector3;
		Cell *quadrupleConnector4;
		
		/* Direction of a transition node.
		 * 'e': east
		 * 'w': west
		 * 'n': north
		 * 's': south
		 * 'f': front
		 * 'b': back
		 */
		char direction;

		friend class Grid;
		friend class CellNode;
	
	public:
		TransitionNode();
		~TransitionNode();
		void print();
};

#endif /*CELL_H_*/
