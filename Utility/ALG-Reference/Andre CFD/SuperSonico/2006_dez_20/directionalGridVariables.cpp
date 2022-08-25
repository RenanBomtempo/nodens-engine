//directionalGridVariables.cpp
/* 
   Updates directional values of physical variables (velocity, density, pressure
   and temperature) at each cell due to its neighbors.
*/




/* ---------- ARQUIVO TOTALMENTE OBSOLETO PARA A VERSAO DE 30/11/06 --------- */





#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cstdlib>
#include <cmath>

/*______________________________________________________________________________
	                       
                            FUNCTION setGridVariables()
								
    Traverses the grid, updating the physical variables in each cell 
    for each of its four directions (north, south, east, west). Each direction
    is influenced by all neighboring cells in that direction.
    In order to do this, calls function setCellVariables() for each neighbor.
    It is also called by those functions responsible for solving the Navier-Stokes
    equations and correspondingly updating the physical variables according to
    the solutions found.
    
______________________________________________________________________________*/
void setGridVariables( Grid *grid )
{
    Cell *gridCell,
         *neighborGridCell;
    char direction;
        
    //Updates interior cell directional physical variables    
    gridCell = grid->firstGridCell;
    while ( gridCell != 0 )
    {
        if ( gridCell->active )
           {
               neighborGridCell = gridCell->south;
               direction = 's';
               setCellVariables( grid, gridCell, neighborGridCell, direction);

               neighborGridCell = gridCell->north;
               direction = 'n';
               setCellVariables( grid, gridCell, neighborGridCell, direction);              
               
               neighborGridCell = gridCell->east;
               direction = 'e';
               setCellVariables( grid, gridCell, neighborGridCell, direction);
               
               neighborGridCell = gridCell->west;
               direction = 'w';
               setCellVariables( grid, gridCell, neighborGridCell, direction );
                                             
           }
        gridCell = gridCell->next;
    }

    //Updates boundary cell directional physical variables
    gridCell = grid->firstGridCell;
    while ( gridCell != 0 )
    {
        if ( gridCell->active )
           {
               neighborGridCell = gridCell->south;
               direction = 's';
               setCellBoundaryVariables( grid, gridCell, neighborGridCell, direction);

               neighborGridCell = gridCell->north;
               direction = 'n';
               setCellBoundaryVariables( grid, gridCell, neighborGridCell, direction);              
               
               neighborGridCell = gridCell->east;
               direction = 'e';
               setCellBoundaryVariables( grid, gridCell, neighborGridCell, direction);
               
               neighborGridCell = gridCell->west;
               direction = 'w';
               setCellBoundaryVariables( grid, gridCell, neighborGridCell, direction );
                                             
           }
        gridCell = gridCell->next;
    }
}


/*______________________________________________________________________________
________________________________________________________________________________
 
                            FUNCTION setCellVariables()

     Computes 
         gridCell->northVariable, 
         gridCell->southVariable, 
         gridCell->eastVariable and
         gridCell->westVariable.
     Its arguments are the current gridCell, its neighbor related to which the 
     directional physical variable is going to be computed and the direction of 
     the neighbor relative to gridCell. This functions only computes for
     interior cells.      
______________________________________________________________________________*/

void setCellVariables( Grid *grid, Cell *gridCell, Cell *neighborGridCell, char direction )
{
  	/* If neighborGridCell is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new neighborGridCell.
  	   This is repeatedly done until a cell node is found. */
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
   	
    //Enters here almost always, except in the case indicated below.  
    if( ( neighborGridCell->type == 'b' ) && ( neighborGridCell->active == true ) )
    {            
              switch (direction)
              {
                case 's':        
                  gridCell->southFaceU = neighborGridCell->uIterAnterior;
                  gridCell->southFaceV = neighborGridCell->vIterAnterior;
                  gridCell->southFaceP = neighborGridCell->pIterAnterior;
                  gridCell->southFaceRho = neighborGridCell->rhoIterAnterior;
                  gridCell->southFaceT = neighborGridCell->TIterAnterior;
                  gridCell->southFaceE = neighborGridCell->eIterAnterior;
                  gridCell->southFaceK = neighborGridCell->kIterAnterior;
                  gridCell->southFaceMi = neighborGridCell->miIterAnterior; 
                break;

                case 'n':            
                  gridCell->northFaceU = neighborGridCell->uIterAnterior;
                  gridCell->northFaceV = neighborGridCell->vIterAnterior;
                  gridCell->northFaceP = neighborGridCell->pIterAnterior;
                  gridCell->northFaceRho = neighborGridCell->rhoIterAnterior;
                  gridCell->northFaceT = neighborGridCell->TIterAnterior;
                  gridCell->northFaceE = neighborGridCell->eIterAnterior;
                  gridCell->northFaceK = neighborGridCell->kIterAnterior;
                  gridCell->northFaceMi = neighborGridCell->miIterAnterior;                  
                break;

                case 'e':
                  gridCell->eastFaceU = neighborGridCell->uIterAnterior;
                  gridCell->eastFaceV = neighborGridCell->vIterAnterior;
                  gridCell->eastFaceP = neighborGridCell->pIterAnterior;
                  gridCell->eastFaceRho = neighborGridCell->rhoIterAnterior;
                  gridCell->eastFaceT = neighborGridCell->TIterAnterior;
                  gridCell->eastFaceE = neighborGridCell->eIterAnterior;
                  gridCell->eastFaceK = neighborGridCell->kIterAnterior;
                  gridCell->eastFaceMi = neighborGridCell->miIterAnterior;
                break;

                case 'w':            
                  gridCell->westFaceU = neighborGridCell->uIterAnterior;
                  gridCell->westFaceV = neighborGridCell->vIterAnterior;
                  gridCell->westFaceP = neighborGridCell->pIterAnterior;
                  gridCell->westFaceRho = neighborGridCell->rhoIterAnterior;
                  gridCell->westFaceT = neighborGridCell->TIterAnterior;
                  gridCell->westFaceE = neighborGridCell->eIterAnterior;
                  gridCell->westFaceK = neighborGridCell->kIterAnterior;
                  gridCell->westFaceMi = neighborGridCell->miIterAnterior;                
                break;
              
                default: break;
              }                            
    }
 
}

/*______________________________________________________________________________
________________________________________________________________________________
 
                            FUNCTION setCellBoundaryVariables()

     Computes 
         gridCell->northVariable, 
         gridCell->southVariable, 
         gridCell->eastVariable and
         gridCell->westVariable.
     Its arguments are the current gridCell, its neighbor related to which the 
     directional physical variable is going to be computed and the direction of 
     the neighbor relative to gridCell. This functions only computes for
     boundary cells.   
______________________________________________________________________________*/

void setCellBoundaryVariables( Grid *grid, Cell *gridCell, Cell *neighborGridCell, char direction )
{
    /* If neighborGridCell is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new neighborGridCell.
  	   This is repeatedly done until a cell node is found. */
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


    /* It enters this "if" only if neighborGridCell is connected to null, i.e.,
       only if gridCell belongs to the boundary of the domain. Then it calls
       function set<Direction>BoundaryCellVariables() in order to set the physical
       variables for this cell. */
    if( ( neighborGridCell->type == 'w' ) || ( neighborGridCell->active == false ) )
    {
        switch(direction)
        {
            case 's':      
                setSouthBoundaryCellVariables( gridCell );
            break;
        
            case 'n': 
                setNorthBoundaryCellVariables( gridCell );
            break;
    
            case 'e':
                setEastBoundaryCellVariables( gridCell );
            break;

            case 'w':
                setWestBoundaryCellVariables( gridCell );
            break;
            
            default: break;
        }
    }
}
