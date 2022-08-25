/* Grid.h
Definition of class Grid.
An object Grid represents a square grid. This class is a singleton.
*/

#ifndef GRID_H
#define GRID_H

#include "Cell.h"
#include "table.h"

class Grid {    
    public:
     
        Grid();		 //constructor
        ~Grid();	 //destructor
        
        Cell *firstGridCell;
        int numberOfGroups; //number of groups in the grid.
        double sideLength;	//Length of square grid. Default = 1.0.
        int numberOfCells;   //Number of cells of grid.
        int **Matrix; //Matrix of groups
   
        void initializeGrid( );
        void refineGridInitially( int );
        void drawHilbertCurve( int );
        void drawLineOfGroups( int );
        void drawSuperGroups( int );
        void printNumberOfSuperGroups( int );
        void writeCellsNumber( int color );
        void drawGrid( int );
        void orderGridCells();
        void getWhitePoints( Table* );
        
        void MatchPoints( Table*, double MAX );
        double Distance( Cell*, Cell*, Table* );
        double SimpleDistance( double X1, double Y1, double X2, double Y2 );
        Cell* FindNeighbor( Cell* , Cell *);
        void CreateMatrix();
        void MatchGroups( int N, double SUPERMAX );
        void AtualizeMatrix(int, int);
        void PrintMatrix(char *nome);
        void PrintCellsSuperGroups(char *fileName);
        
        void completeToMakeBlackCurve(Table* blackTable);
        void turnBackGridPosition( int i );
        void drawBlackCurve( int color );
        void printCells(char *fileName, Table *blackTable);
        
};//end class Grid

#endif
