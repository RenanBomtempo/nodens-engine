/* Grid.h
Definition of class Grid.
An object Grid represents a square grid. This class is a singleton.
*/

#ifndef GRID_H
#define GRID_H

#include "Cell.h"


class Grid
{    
    public:
        
        Grid();				 //constructor
        ~Grid();			 //destructor
        
        Cell *firstGridCell;   
        double sideLength;	//Length of square grid. Default = 1.0.
        int numberOfCells;  //Number of cells of grid.
        
        //Variables used in modelling constant first-order wave equation u_t + cu_x = 0.
        double deltaT,     //time increment
               velocityX,  //x-coordinate of vector c in wave equation
               velocityY;  //y-coordinate of vector c in wave equation   
        
        void initializeGrid( bool );
        void refineGridInitially( int, bool );
        void drawHilbertCurve( int );
        void orderGridCells();
               
        //Print functions.
        int print();
        
        //Method used in solving the wave equation.
        void solveWave( bool );
        
        //Flag for helping debug logical errors.
        bool globalFlag;
    
};//end class Grid


#endif
