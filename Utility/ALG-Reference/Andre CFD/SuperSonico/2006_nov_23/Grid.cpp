/* Grid.cpp
Implementation of class Grid. */

// Bibliotecas basicas
#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;

#include <new>

#include <fstream>
    using std::ofstream;

#include <math.h>

// Arquivos do meu projeto
#include "Grid.h"
#include "functions.h"
#include "constants.h"

//Constructor
Grid::Grid():
    firstGridCell(0),
    sideLength( LHORI ),
    numberOfCells(1),
    deltaT(DELTA_T),
    globalFlag(false),
    tempoAtual(0)
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
           
        drawLine(0.0,1.0,0,0.0,0.0,0);	//Draws square's west border.
        drawLine(1.0,1.0,0,0.0,1.0,0);	//Draws square's north border.
        drawLine(0.0,0.0,0,1.0,0.0,0);	//Draws square's south border.
        drawLine(1.0,0.0,0,1.0,1.0,0);	//Draws square's east border.
        drawLine(0.0,0.5,0,1.0,0.5,0);	//Draws horizontal line inside square.
        drawLine(0.5,0.0,0,0.5,1.0,0);	//Draws vertical line inside square.
        
        drawHilbertCurve(1);			//Draws Hilbert curve in red color.
    }
    
    orderGridCells();
    
}//end initializeGrid() function


/*
______________________________________________________________________________
                           
                           PEANO'S CURVE DRAWING
                        	
    Draws peano curve of whole grid.
______________________________________________________________________________*/
void Grid::drawPeanoCurve( int colorValue )
{
    Cell *gridCell;
    
    setColor( colorValue );

    gridCell = firstGridCell;
    while( gridCell->next != 0 )
    {
        drawLine( gridCell->centerX/sideLength, 
                  gridCell->centerY/sideLength, 
                  0,
                     gridCell->next->centerX/sideLength, 
                     gridCell->next->centerY/sideLength,
                     0);
        
        gridCell = gridCell->next;
    }

}//end drawPeanoCurve() function.



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
        drawLine( gridCell->centerX, gridCell->centerY,0,
                  gridCell->next->centerX, gridCell->next->centerY,0 );
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
    
    ofstream outputFile( "outputESSPP.txt", ios::app ); //trunc
    
    int counter = 1;
    gridCell = firstGridCell;
    while( gridCell != 0 )
   	{
   	    //outputFile << "Cell " << counter << "\tu: " << gridCell->u << endl;
                                    
        /*Alterado pelo Andre STALTZ*/
        if(gridCell->type == 'b')
           outputFile << "centro: " << gridCell->centerX 
                      << "," << gridCell->centerY 
                      << " u: " << gridCell->u << endl;
        
        counter++;
        gridCell = gridCell->next;
    }
    outputFile << 
    "__________________________________________________________________________"
    << endl;
        
}//end print() function.


/*______________________________________________________________________________
      
                        FUNCTION initialGuess()
                        
   Sets initial shot of the conjugate gradient method.
   The initial vector is a linear interpolation of boundary values.
______________________________________________________________________________*/
void Grid::initialGuess()
{
    Cell *gridCell;
    gridCell = firstGridCell;
    
    while( gridCell != 0 )
    {
        gridCell->u = 
                (( gridCell->centerY * 0 )
              + ( ( 1.0 - gridCell->centerY ) * 0 )
              + ( ( 1.0 - gridCell->centerX ) * 0 )
              +  gridCell->centerX * 0 );
        gridCell = gridCell->next; 
    }
}

/*______________________________________________________________________________
  
                        FUNCTION initializeDiagonalElements()
                        
  Initializes the diagonal elements of the grid discretization matrix.
  Each diagonal element corresponds to the first element in the linked list which 
  represents the row associated to the particular cell.
  
  Moreover, this function also resets to zero the vector b in Ax = b.

______________________________________________________________________________*/
void Grid::initializeDiagonalElements()
{
    Cell *gridCell;
    gridCell = firstGridCell;
    
    Element *element;
    while( gridCell != 0 )
   	{
   	    if ( gridCell->active )
   	    {
            gridCell->b = 2*gridCell->u - gridCell->previousU;
            gridCell->previousU = gridCell->u;

            element = new Element;
       	    element->column = gridCell->gridPosition;
       	    element->cell = gridCell;
            element->value=1;
            gridCell->firstElement = element;
       	}    
   	    gridCell = gridCell->next;          	    
    } 
}

/*______________________________________________________________________________
                      
                      FUNCTION setDiscretizationMatrix()
  
    Initializes the non-diagonal entries of the grid discretization matrix.
    Determines the values of all entries in A and b by calling function
    fillDiscretizationMatrixElements(), defined immediately afterwards, for each
    of its 4 neighbor nodes (either cell nodes or transition nodes).

______________________________________________________________________________*/
void Grid::setDiscretizationMatrix()
{ 
    int n = numberOfCells;
    Cell *gridCell,
         *neighborGridCell;
    char direction;

    setColor(7);
    initializeDiagonalElements();
    
    gridCell = firstGridCell;
    while ( gridCell != 0 )
    {
        if ( gridCell->active )
        {               
               //Computes and designates the flux due to south cells.
               neighborGridCell = gridCell->south;
               direction = 's';
               fillDiscretizationMatrixElements(gridCell, neighborGridCell, direction );
               //Computes and designates the flux due to north cells.

               neighborGridCell = gridCell->north;
               direction = 'n';
               fillDiscretizationMatrixElements(gridCell, neighborGridCell, direction);
               
               //Computes and designates the flux due to east cells.
               neighborGridCell = gridCell->east;
               direction = 'e';
               fillDiscretizationMatrixElements(gridCell, neighborGridCell, direction);
               
               //Computes and designates the flux due to west cells.
               neighborGridCell = gridCell->west;
               direction = 'w';
               fillDiscretizationMatrixElements(gridCell, neighborGridCell, direction);

        }  
        gridCell = gridCell->next;
    }


}

/*______________________________________________________________________________
                      
                      FUNCTION fillDiscretizationMatrixElements()   
______________________________________________________________________________*/
void Grid::fillDiscretizationMatrixElements( 
     Cell *gridCell, Cell *neighborGridCell, char direction )
{    
    int twoToCellLevel = 1,               /* 2^(refinement level of gridCell) */
        twoToLevelDifference = 1,         /* 2^(difference between refinement levels 
                                                of gridCell and neighborGridCell) */
        levelDifference,
        position;                           //Coluna da célula vizinha.
    double boundaryValue, fr;
    
  	//If neighborGridCell is a transition node, considers the cell to which its
    //doubleConnector1 points to, which will be the new neighborGridCell.
  	//This is repeatedly done until a cell node is found.
  	if ( neighborGridCell->level > gridCell->level )
    	while( ( neighborGridCell->type == 'w' ) && 
               ( neighborGridCell->singleConnector != 0 ) 
             )
    	{
        	neighborGridCell = neighborGridCell->doubleConnector1;
    	}   	
  	else
    	while( ( neighborGridCell->type == 'w' ) && 
               ( neighborGridCell->singleConnector != 0 ) 
             )
    	{
    		neighborGridCell = neighborGridCell->singleConnector;
   		}
    

    //Fills the constant term b and the diagonal entry corresponding to boundary
    //cells (i.e., if neighborGridCell is a transition node pointing to null (i.e., ground).
    if( ( neighborGridCell->type == 'w' ) || ( neighborGridCell->active == false ) )
    {
        int i = gridCell->level;
        while( i > 0 )
        {
            twoToCellLevel *= 2;
            i--;
        }
        if( direction == 's')      
        {                   
            // CONDICOES DE FRONTEIRA ??
        }
        else if( direction == 'n' ) 
        {
            // CONDICOES DE FRONTEIRA ??
        }
        else if( direction == 'e' ) 
        {   
            // CONDICOES DE FRONTEIRA ??
        }
        else if( direction == 'w' ) 
        {
            // CONDICOES DE FRONTEIRA ??
        }

        gridCell->firstElement->value += 2.0 / (double)twoToCellLevel;
        gridCell->firstElement->column = gridCell->gridPosition;
    }
    //Fills entries of remaining cells.  
    else  if( ( neighborGridCell->type == 'b' ) && ( neighborGridCell->active == true ) )
    {
        if( (gridCell->level > neighborGridCell->level) || //Avoids duplicating the computation.
    		  ( (gridCell->level == neighborGridCell->level) &&
    		  /*Follows the dictionary order, in order to avoid duplicating the computation.*/
            	( (gridCell->centerX > neighborGridCell->centerX) ||
           		  ( ( gridCell->centerX == neighborGridCell->centerX ) && 
                              ( gridCell->centerY > neighborGridCell->centerY ) )
                )
              )
            )
        {
              levelDifference = gridCell->level - neighborGridCell->level; 
              while ( levelDifference > 0 )
              {
                  twoToLevelDifference *= 2;
                  levelDifference--;
              }
              int i = gridCell->level;
              while( i > 0 )
              {
                  twoToCellLevel *= 2;
                  i--;
              }
              fr = 1.0 / (double)twoToLevelDifference;//Allows the matrix to be symmetric.
              fr /= (double)twoToCellLevel;

              gridCell->firstElement->value += fr;
              neighborGridCell->firstElement->value += fr;
              
              Element *element;              
              element = gridCell->firstElement;
              position = neighborGridCell->gridPosition;
              while( element != 0 && element->column != position )
              {
                  element = element->next;
              }
              if( element != 0 )
              {
                  element->value -= fr;
              }
              else
              {
                  element = new Element; 
                  element->column = position;
                  element->value = -fr;
                  element->cell = neighborGridCell;
                  Element *auxiliarElement;
                  auxiliarElement = gridCell->firstElement->next;
                  gridCell->firstElement->next = element;
                  element->next = auxiliarElement;                 
              }
              
              element = neighborGridCell->firstElement;
              position = gridCell->gridPosition;
              while( element != 0 && element->column != position )
              {
                  element = element->next;
              }
              if( element != 0 )
              {
                  element->value -= fr;
              }
              else
              {
                  element = new Element; 
                  element->column = position;
                  element->value = -fr;
                  element->cell = gridCell;
                  Element *auxiliarElement;
                  auxiliarElement = neighborGridCell->firstElement->next;
                  neighborGridCell->firstElement->next = element;
                  element->next = auxiliarElement;                                                       
              }
               
        }
    }   		
}

/*______________________________________________________________________________

                        FUNCTION conjugateGradient()
                        
    Conjugate gradient method for symmetric positive definite matrix.
    The grid discretization matrix and all variables used in the conjugate
    gradient method are stored on the grid's cells, which improves performance. 
    There is no independent linear algebra package.
    
______________________________________________________________________________*/
void Grid::conjugateGradient()
{
    double error = 0.0,	     //Norm of resídue b - Ax after each iteration.
           pError = 1.0,
           rTr,
           r1Tr1,
           pTAp,
           alpha,
           beta,
           normR = 0.0, 
           normPP1 = 0.0,
           precision = 1e-15; //Maximum precision which error must not exceed.
           
    int maximumNumberOfIterations = numberOfCells,
        numberOfIterations = 0,
        column;
        
    Cell *gridCell;
    Element *element;
        
    //__________________________________________________________________________    
    //Computes vector A*x, residue r = b - Ax, scalar rTr = r^T * r and 
    //sets initial search direction p.    
    gridCell = firstGridCell; 
    rTr = 0.0;   
    while( gridCell != 0 )
    {
        if( gridCell->active )
        {
            gridCell->Ax = 0.0;
            element = gridCell->firstElement;
            while( element != 0 )
            {
                gridCell->Ax += element->value * element->cell->u;
                element = element->next;           
            }
            gridCell->r = gridCell->b - gridCell->Ax;
            rTr += gridCell->r * gridCell->r;
            gridCell->p = gridCell->r;
            if( fabs(gridCell->r) > error )
                error = fabs(gridCell->r);
        }    
        gridCell = gridCell->next;
    }    
    //__________________________________________________________________________
    //Conjugate gradient iterations.
    if( error >= precision )
    {
        while( (pError >= precision)  && 
               (numberOfIterations < maximumNumberOfIterations) )
        {
            pError = 0.0;
            normPP1 = 0.0;
            //__________________________________________________________________
            // Computes Ap and pTAp. Uses Ax to store Ap.
            gridCell = firstGridCell;
            pTAp = 0.0;    
            while( gridCell != 0 )
            {
                if( gridCell->active )
                {
                    gridCell->Ax = 0.0;
                    element = gridCell->firstElement;
                    while( element != 0 )
                    {
                        gridCell->Ax += element->value * element->cell->p;
                        element = element->next;           
                    }
                    pTAp += gridCell->p * gridCell->Ax;                        
                }    
                gridCell = gridCell->next;
            }
            //__________________________________________________________________
            // Computes alpha.
            alpha = rTr/pTAp;
            //__________________________________________________________________
            // Computes new value of solution: u = u + alpha*p.
            gridCell = firstGridCell;    
            while( gridCell != 0 )
            {
                if( gridCell->active )
                    gridCell->u += alpha * gridCell->p;
                gridCell = gridCell->next;
            }
            //__________________________________________________________________
            //Upgrades residue r1 = r - alpha*Ap and computes r1Tr1 = r1^T * r1. 
            gridCell = firstGridCell;
            r1Tr1 = 0.0;    
            while( gridCell != 0 )
            {
                if( gridCell->active )
                {                
                    gridCell->r -= alpha * gridCell->Ax;
                    r1Tr1 += gridCell->r * gridCell->r;
                }                       
                gridCell = gridCell->next;
            }
            //__________________________________________________________________            
            //Computes beta.
            beta = r1Tr1/rTr;
            //__________________________________________________________________            
            //Computes vector p1 = r1 + beta*p and uses it to upgrade p.
            gridCell = firstGridCell;    
            while( gridCell != 0 )
            {
                if( gridCell->active )
                {                
                    gridCell->p1 = gridCell->r + beta * gridCell->p;   
                    if ( fabs(gridCell->r) > normR )
                         normR = fabs(gridCell->r);
                    if ( fabs(gridCell->p - gridCell->p1) > normPP1 )
                         normPP1 = fabs(gridCell->p - gridCell->p1);
                    gridCell->p = gridCell->p1;
                }                    
                gridCell = gridCell->next;
            }
            
            if( normR < precision )
                pError = 0.0;
            else if( normPP1 > pError )
                pError = normPP1;
            //__________________________________________________________________
            //Upgrades rTr.
            rTr = r1Tr1;

            numberOfIterations++;
       }  
     
     cout << "Numero de firstGridCells: " << numberOfCells
     << "\tNumero de iteracoes do gradiente conjugado: " << numberOfIterations << endl;  
     }//end of conjugate gradient iterations.
}//end conjugateGradient() function.


/*______________________________________________________________________________

                        FUNCTION freeMemory()
                        
    Eliminates memory overload of grid discretization matrix at each refinement 
    iteration.
    
______________________________________________________________________________*/
void Grid::freeMemory()
{
    Cell *gridCell;
    Element *element,
            *auxiliarElement;
            
    gridCell = firstGridCell;    
    while( gridCell != 0 )
    {
        if( gridCell->active )
        {
            element = gridCell->firstElement;
              
            while( element != 0 )
            {
                auxiliarElement = element->next; 
                delete element;
                element = auxiliarElement;    
            }
        }        
        gridCell = gridCell->next;
     }    
}//end freeMemory() function.
                                         
