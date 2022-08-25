//flux.cpp

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>

/*______________________________________________________________________________

								FUNCTION setGridFlux()
								
    Traverses the grid, computing the fluxes in all directions (north, south, 
    east, west) due to all neighboring cells in order to compute the total flux.
    In order to do this, calls function setCellFlux().	
	
______________________________________________________________________________*/
void setGridFlux( Grid *grid )
{
    Cell *gridCell,
         *neighborGridCell;
    char direction;  
    
    gridCell = grid->firstGridCell;
    while ( gridCell != 0 )
    {
        gridCell->northFlux = 0.0;
        gridCell->southFlux = 0.0;
        gridCell->eastFlux = 0.0;
        gridCell->westFlux = 0.0;
        
        gridCell = gridCell->next;
    }        
        
    gridCell = grid->firstGridCell;
    while ( gridCell != 0 )
    {
       //Computes southFlux.
       neighborGridCell = gridCell->south;
       direction = 's';
       setCellFlux( gridCell, neighborGridCell, direction);
       
       //Computes northFlux.
       neighborGridCell = gridCell->north;
       direction = 'n';
       setCellFlux( gridCell, neighborGridCell, direction);              
       
       //Computes eastFlux.
       neighborGridCell = gridCell->east;
       direction = 'e';
       setCellFlux( gridCell, neighborGridCell, direction);
       
       //Computes westFlux.
       neighborGridCell = gridCell->west;
       direction = 'w';
       setCellFlux( gridCell, neighborGridCell, direction );
                                             
        gridCell = gridCell->next;
    }
}


/*______________________________________________________________________________
    
                            FUNCTION setCellFlux()
    
    Computes the flux into this cell due to a neighbor cell.
	 
    flux = (u(thisCell) - u(neighborCell)/2* leastDistance,
    
    where 2* leastDistance is two times the least of the lengths of the faces of
	this cell and its neighbor.
    
______________________________________________________________________________*/
void setCellFlux( Cell *gridCell, Cell *neighborGridCell, char direction )
{
    int levelDifference, //difference between levels of gridCell and neighborGridCell
        twoToLevelDifference = 1; //2*levelDifference
    
	double leastDistance = gridCell->halfFaceLength,
       	   localFlux;
  	
  	//When neighborGridCell is a transition node, looks for the next neighbor 
    //cell which is a cell node.
  	if ( neighborGridCell->level > gridCell->level )
    	while( ( neighborGridCell->type == 'w' ) && 
               ( neighborGridCell->singleConnector != 0 ) 
             )
    	{
        	neighborGridCell = neighborGridCell->doubleConnector2;
    	}   	
  	else
    	while( ( neighborGridCell->type == 'w' ) && 
               ( neighborGridCell->singleConnector != 0 ) 
             )
    	{
    	    neighborGridCell = neighborGridCell->singleConnector;
   		}

    //It enters this "if" only if neighborGridCell is connected to null, i.e.,
    //only if gridCell belongs to the boundary of the domain. 
    if( neighborGridCell->type == 'w' )
    {        
        localFlux = gridCell->u / gridCell->halfFaceLength;
        
        if( direction == 's')      
        {
            if ( localFlux > gridCell->southFlux )
                 gridCell->southFlux = localFlux;
        }
        else if( direction == 'n' ) 
        {
            if ( localFlux > gridCell->northFlux )
                 gridCell->northFlux = localFlux;
        }
        else if( direction == 'e' ) 
        {
            if ( localFlux > gridCell->eastFlux )
                 gridCell->eastFlux = localFlux;
        }
        else if( direction == 'w' ) 
        {
            if ( localFlux > gridCell->westFlux )
                 gridCell->westFlux = localFlux;            
        }
    }
    //Enters here almost always, except in the case indicated above.  
    else if( neighborGridCell->type == 'b' )
    {
        //Dictionary order avoids computing the flux twice for the same cell.
        if( (gridCell->level > neighborGridCell->level) ||
    		  ( (gridCell->level == neighborGridCell->level) &&
            	( (gridCell->centerX > neighborGridCell->centerX) ||
           		  ( ( gridCell->centerX == neighborGridCell->centerX ) && 
                              ( gridCell->centerY > neighborGridCell->centerY ) )
                )
              )
            )        
        
        levelDifference = gridCell->level - neighborGridCell->level; 
        while ( levelDifference > 0 )
        {
             twoToLevelDifference *= 2;
             levelDifference--;
        }
        
        
        if ( neighborGridCell->halfFaceLength < leastDistance )
             leastDistance = neighborGridCell->halfFaceLength;
              
        localFlux = ( gridCell->u - neighborGridCell->u )/ (2*leastDistance);
              
        if( direction == 's')      
        {
            if ( localFlux > gridCell->southFlux )
                 gridCell->southFlux += localFlux;
            if ( localFlux > neighborGridCell->northFlux )
                 neighborGridCell->northFlux += localFlux;
        }
        else if( direction == 'n' ) 
        {
            if ( localFlux > gridCell->northFlux )
                 gridCell->northFlux += localFlux;
            if ( localFlux > neighborGridCell->southFlux )
                 neighborGridCell->southFlux += localFlux;
        }
        else if( direction == 'e' ) 
        {
            if ( localFlux > gridCell->eastFlux )
                 gridCell->eastFlux += localFlux;
            if ( localFlux > neighborGridCell->westFlux )
                 neighborGridCell->westFlux += localFlux;
        }
        else if( direction == 'w' ) 
        {
            if ( localFlux > gridCell->westFlux )
                 gridCell->westFlux += localFlux;
            if ( localFlux > neighborGridCell->eastFlux )
                 neighborGridCell->eastFlux += localFlux;            
        }
        
    }
}

