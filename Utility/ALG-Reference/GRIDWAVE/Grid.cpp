/* Grid.cpp
Implementation of class Grid. */

#include "Grid.h"
#include "functions.h"

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;

#include <new>
    
#include <fstream>
    using std::ofstream;
    
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <gl/glu.h>

//Constructor
Grid::Grid():
    firstGridCell(0),
    sideLength(1.0),
    numberOfCells(1),
    deltaT(0),
    velocityX(0),
    velocityY(0),
    globalFlag(false)
{}

//Destructor
Grid::~Grid()
{}

/*______________________________________________________________________________
________________________________________________________________________________
                           
                           FUNCTION initializeGrid()

    Initializes grid with four cells.
    
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::initializeGrid( bool drawGrid )
{
    //Subcells.
    Cell *northeastCell,
    	 *northwestCell,
  		 *southwestCell,
     	 *southeastCell;
    
    northeastCell = new Cell;
    northwestCell = new Cell;
    southeastCell = new Cell;
    southwestCell = new Cell;    
    
    //Ground transition nodes.     
    Cell *northTransitionNode,
   		 *southTransitionNode,
      	 *eastTransitionNode,
         *westTransitionNode;
         
    northTransitionNode = new Cell;	
    southTransitionNode = new Cell;
    eastTransitionNode = new Cell;	
    westTransitionNode = new Cell;
         
    //Grid geometry.
    double halfSideLength = sideLength/2.0,	
  		   quarterSideLength = sideLength/4.0;
  		   
       	
    //__________________________________________________________________________
    //					Initialization of ground transition nodes.
    //__________________________________________________________________________
  
  	eastTransitionNode->type = 'w';
   	eastTransitionNode->level = 1;
   	eastTransitionNode->active = true;
    eastTransitionNode->singleConnector = 0;
    eastTransitionNode->doubleConnector1 = northeastCell;
    eastTransitionNode->doubleConnector2 = southeastCell;
    
  	northTransitionNode->type = 'w';
 	northTransitionNode->level = 1; 
 	northTransitionNode->active = true;
    northTransitionNode->singleConnector = 0; 
   	northTransitionNode->doubleConnector1 = northeastCell;
    northTransitionNode->doubleConnector2 = northwestCell;   
    
   	westTransitionNode->type = 'w';
   	westTransitionNode->level = 1;
   	westTransitionNode->active = true;
    westTransitionNode->singleConnector = 0;
    westTransitionNode->doubleConnector1 = northwestCell;
    westTransitionNode->doubleConnector2 = southwestCell;
    
   	southTransitionNode->type = 'w';
  	southTransitionNode->level = 1;
  	southTransitionNode->active = true;
    southTransitionNode->singleConnector = 0;
    southTransitionNode->doubleConnector1 = southeastCell;
    southTransitionNode->doubleConnector2 = southwestCell;
 
    //__________________________________________________________________________
    //					  Initialization of cell nodes.
    //__________________________________________________________________________
 
    //Northeast subcell initialization.  
 	northeastCell->type = 'b';
  	northeastCell->active = true;
  	northeastCell->level = 1;
  	northeastCell->bunchNumber = 1;
  	northeastCell->faceLength = halfSideLength;
  	northeastCell->halfFaceLength = quarterSideLength;
  	
  	northeastCell->east = eastTransitionNode;
  	northeastCell->north = northTransitionNode;
  	northeastCell->west = northwestCell;
   	northeastCell->south = southeastCell;
   	northeastCell->previous = 0;
   	northeastCell->next = northwestCell;
   	
   	northeastCell->gridPosition = 0;
    northeastCell->hilbertShapeNumber = 0;
    
   	northeastCell->centerX = halfSideLength + quarterSideLength;
   	northeastCell->centerY = halfSideLength + quarterSideLength;

    //Northwest subcell initialization.  
  	northwestCell->type = 'b';
   	northwestCell->active = true;
   	northwestCell->level = 1;
   	northwestCell->bunchNumber = 2;
  	northwestCell->faceLength = halfSideLength;
  	northwestCell->halfFaceLength = quarterSideLength;
  	
  	northwestCell->east = northeastCell;
  	northwestCell->north = northTransitionNode;
  	northwestCell->west = westTransitionNode;
   	northwestCell->south = southwestCell;
   	northwestCell->previous = northeastCell;
   	northwestCell->next = southwestCell;
 
	northwestCell->gridPosition = 1;
	northwestCell->hilbertShapeNumber = 1;
    
   	northwestCell->centerX = halfSideLength - quarterSideLength;
   	northwestCell->centerY = halfSideLength + quarterSideLength;
     
    //Southwest subcell initialization.  
  	southwestCell->type = 'b';
   	southwestCell->active = true;
   	southwestCell->level = 1;
   	southwestCell->bunchNumber = 3;
  	southwestCell->faceLength = halfSideLength;
  	southwestCell->halfFaceLength = quarterSideLength;
  	
  	southwestCell->east = southeastCell;
  	southwestCell->north = northwestCell;
  	southwestCell->west = westTransitionNode;
   	southwestCell->south = southTransitionNode;
   	southwestCell->previous = northwestCell;
   	southwestCell->next = southeastCell;
   	
   	southwestCell->gridPosition = 2;
	southwestCell->hilbertShapeNumber = 2;
    
   	southwestCell->centerX = halfSideLength - quarterSideLength;
   	southwestCell->centerY = halfSideLength - quarterSideLength;
  
    //Southeast subcell initialization.  
  	southeastCell->type = 'b';
   	southeastCell->active = true;
   	southeastCell->level = 1;
   	southeastCell->bunchNumber = 4;
  	southeastCell->faceLength = halfSideLength;
  	southeastCell->halfFaceLength = quarterSideLength;
  	
  	southeastCell->east = eastTransitionNode;
  	southeastCell->north = northeastCell;
  	southeastCell->west = southwestCell;
   	southeastCell->south = southTransitionNode;
   	southeastCell->previous = southwestCell;
   	southeastCell->next = 0;
   	
   	southeastCell->gridPosition = 3;
	southeastCell->hilbertShapeNumber = 3;
    
   	southeastCell->centerX = halfSideLength + quarterSideLength;
   	southeastCell->centerY = halfSideLength - quarterSideLength;
   	
    //Grid initialization
    firstGridCell = northeastCell;        
    numberOfCells = 4;
    
    //__________________________________________________________________________
    //Grid drawing.
    if( drawGrid )
    {
        setColor(5);		        //Sets drawing color to blue
           
        drawLine(0.0,1.0,0.0,0.0);	//Draws square's west border.
        drawLine(1.0,1.0,0.0,1.0);	//Draws square's north border.
        drawLine(0.0,0.0,1.0,0.0);	//Draws square's south border.
        drawLine(1.0,0.0,1.0,1.0);	//Draws square's east border.
        drawLine(0.0,0.5,1.0,0.5);	//Draws horizontal line inside square.
        drawLine(0.5,0.0,0.5,1.0);	//Draws vertical line inside square.
        
        drawHilbertCurve(1);			//Draws Hilbert curve in red color.
    }
    
    orderGridCells();
    
}//end initializeGrid() function

/*______________________________________________________________________________
________________________________________________________________________________
                           
                           FUNCTION refineGridInitially()

    Refines grid before starting computations.
    
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::refineGridInitially( int finalLevel, bool drawGrid )
{
 	 Cell *gridCell;    
     for( int i = 0; i < finalLevel; i++ )
     {
         gridCell = firstGridCell;
         while( gridCell != 0 )
       	 {
       	     if( gridCell->level < finalLevel )
       	     {
                 refineCell(gridCell,drawGrid);
                 numberOfCells += 3;
             }    
             gridCell = gridCell->next; 	    
         }
     }    
     orderGridCells(); 	 
}//end refineGridInitially() function

/*______________________________________________________________________________
________________________________________________________________________________
                           
                           FUNCTION drawHilbertCurve()
                        	
    Draws Hilbert curve on the whole grid.
    
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::drawHilbertCurve( int colorValue )
{
    Cell *gridCell;
    gridCell = firstGridCell;
    setColor( colorValue );
    while( gridCell->next != 0 )
    {
        drawLine( gridCell->centerX, gridCell->centerY,
                  gridCell->next->centerX, gridCell->next->centerY );
        gridCell = gridCell->next;
    } 
}//end drawHilbertCurve() function.

/*______________________________________________________________________________
________________________________________________________________________________
                           
                            FUNCTION orderGridCells()
                        	
    Enumerates cells of grid according to the ordering given by Hilbert's curve.
    
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::orderGridCells()
{
    Cell *gridCell;
    gridCell = firstGridCell;
    
    int counter = 0;
    while( gridCell != 0 )
   	{
        gridCell->gridPosition = counter;
        gridCell = gridCell->next;
        counter++;   	    
    }
    
}//end orderGridCells() function.

/*______________________________________________________________________________
________________________________________________________________________________
                           
                               FUNCTION print()
                               
________________________________________________________________________________
______________________________________________________________________________*/
int Grid::print()
{
    Cell *gridCell;
    
    ofstream outputFile( "outputWave.txt", ios::app ); //trunc
    
    int counter = 1;
    gridCell = firstGridCell;
    while( gridCell != 0 )
   	{
   	    outputFile << "Cell " << counter <<   "\t\tu: " << gridCell->u << endl;
        counter++;
        gridCell = gridCell->next;
    }
    outputFile << 
    "__________________________________________________________________________"
    << endl;
    outputFile << 
    "__________________________________________________________________________"
    << endl;
        
}//end print() function.

/*______________________________________________________________________________
________________________________________________________________________________
                            
                            FUNCTION solveWave() 
     
     Solves first order wave's equation through the method of characteristics in
     the following steps:
     1) Initializes grid and sets initial value of solution u.
     2) Refines grid initially.
     3) At each time step.
       a) Computes solution by the method of characteristics.
       b) Refines grid according to refinement bound and maximum refinement level
	      allowed.
       c) Derefines grid according to derefinement bound.
        
     Its boolean argument will determine if the program will draw on the screen
     the grid and its associated Hilbert's curve (true) or not (false).
     
________________________________________________________________________________
______________________________________________________________________________*/
void Grid::solveWave( bool drawGrid )
{
    int initialRefinementLevel = 6,//4
	    maximumRefinementLevel = 7;//6
    double refinementBound = 1e-20,
           derefinementBound = 1e-10;
    double timeInstant,
           finalTime = 5.00;
           
    //deltaT e velocitiesXY must be carefully chosen in order for the MMOC to work.
    deltaT = 0.04;       
    velocityX = 0.2;
    velocityY = 0.2;
    
    //Initialize grid and sets initial shot.
    initializeGrid( drawGrid );
    refineGridInitially( initialRefinementLevel, drawGrid);
    initialCondition( this );
    refineGrid( this, maximumRefinementLevel, refinementBound, drawGrid );
    
    //Computations and refinement iterations.
    for( timeInstant = deltaT; timeInstant <= finalTime; timeInstant += deltaT  )
    {
        cout << timeInstant << endl;
        characteristicsMethod( this );
        storePreviousValues( this ); 
               
        refineGrid( this, maximumRefinementLevel, refinementBound, drawGrid );
        if( drawGrid )
        {
            drawHilbertCurve(7);
            drawHilbertCurve(1);
            glFlush();
        }
        cout << "Number of Cells = " << numberOfCells << endl; 
        
        derefineGrid( this, derefinementBound, drawGrid );
        if( drawGrid )
        {
            drawHilbertCurve(7);
            drawHilbertCurve(1);
            glFlush();
        }
        cout << "Number of Cells = " << numberOfCells << endl;
    }    
    
}//end solveWave() function
//end file Grid.cpp

