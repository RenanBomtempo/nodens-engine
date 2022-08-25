//convectiveTerm.cpp

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cstdlib>
#include <cmath>


/*______________________________________________________________________________
    
                            FUNCTION characteristicsMethod()
    
    Computes the solution by the method of characteristics.
    
______________________________________________________________________________*/
void characteristicsMethod( Grid *grid )
{
    Cell *gridCell,
         *walkingGridCell,
         *auxiliarGridCell,
         *neighborGridCell;
    
    double characteristicFoot[2];
    double xFoot,
           yFoot,
           realCenterX ,
           realCenterY,
           xDistance,
           yDistance,
           xDistanceModulus,
           yDistanceModulus,
           halfFace;
    
    gridCell = grid->firstGridCell;    
    while( gridCell != 0 )
    {
        
        findCharacteristicFoot(grid, gridCell, characteristicFoot);
        xFoot = characteristicFoot[0];
        yFoot = characteristicFoot[1];
        
        //If the foot of the characteristic is outside the domain.
        if( xFoot <= 0.0 || yFoot <= 0.0 || xFoot >= 1.0 || yFoot >= 1.0 )
            gridCell->u = 0.0;
        
        //__________________________________________________________________
        //If the foot of the characteristic is inside the domain.
        else
        {                
            realCenterX = gridCell->centerX;  
            realCenterY = gridCell->centerY;
            xDistance = xFoot - realCenterX;
            yDistance = yFoot - realCenterY;
            xDistanceModulus = fabs(xDistance);
            yDistanceModulus = fabs(yDistance);
            halfFace = gridCell->halfFaceLength;
                            
            walkingGridCell = gridCell;
            //______________________________________________________________
            //If the foot of the characteristic is outside the current cell.  
            if( ( xDistanceModulus > halfFace )||
                ( yDistanceModulus > halfFace ) )
            {
                //__________________________________________________________
                //Finds the cell where the characteristic foot is in, by
                //finding the neighbors where the characteristic line passes
                //through. In order to do this, calls the function
                //findsCharacteristicNeighborCell(), which finds the next
                //neighbor through which the characteristic line passes through.
                while( ( xDistanceModulus > halfFace ) ||  
                       ( yDistanceModulus > halfFace ) )
                {                        
                    realCenterX = walkingGridCell->centerX;  
                    realCenterY = walkingGridCell->centerY;
                    auxiliarGridCell = walkingGridCell;
                    
                    
                    
                    
                    if( xDistanceModulus >= yDistanceModulus )
                    {
                        if( realCenterX > xFoot )
                        {
                            walkingGridCell = auxiliarGridCell->west;
                            if( walkingGridCell->type == 'w' )
                            {
                                walkingGridCell = auxiliarGridCell;
                                neighborGridCell = auxiliarGridCell->west;
                                findsCharacteristicNeighborCell(walkingGridCell,
                                                    neighborGridCell);
                                walkingGridCell = neighborGridCell;
                            }                           
                        }
                        else if( realCenterX <= xFoot )
                        {
                            walkingGridCell = auxiliarGridCell->east;
                            if( walkingGridCell->type == 'w' )
                            {
                                walkingGridCell = auxiliarGridCell;
                                neighborGridCell = auxiliarGridCell->east;
                                findsCharacteristicNeighborCell(walkingGridCell,
                                                    neighborGridCell);
                                walkingGridCell = neighborGridCell;
                            }                             
                        }
                        
                    }
                    else if( yDistanceModulus > xDistanceModulus )
                    {
                        if( realCenterY > yFoot )
                        {
                            walkingGridCell = auxiliarGridCell->south;
                            if( walkingGridCell->type == 'w' )
                            {
                                walkingGridCell = auxiliarGridCell;
                                neighborGridCell = auxiliarGridCell->south;
                                findsCharacteristicNeighborCell(walkingGridCell,
                                                    neighborGridCell);
                                walkingGridCell = neighborGridCell;
                            }
                        }
                        else if( realCenterY <= yFoot )
                        {
                            walkingGridCell = auxiliarGridCell->north;
                            if( walkingGridCell->type == 'w' )
                            {
                                walkingGridCell = auxiliarGridCell;
                                neighborGridCell = auxiliarGridCell->north;
                                findsCharacteristicNeighborCell(walkingGridCell,
                                                    neighborGridCell);
                                walkingGridCell = neighborGridCell;
                            }                             
                        }
                    }
                    
                    if( walkingGridCell->type == 'b' )
                    {
                        realCenterX = walkingGridCell->centerX;
                        realCenterY = walkingGridCell->centerY;
                        xDistance = xFoot - realCenterX;
                        yDistance = yFoot - realCenterY;
                        xDistanceModulus = fabs(xDistance);
                        yDistanceModulus = fabs(yDistance);
                        halfFace = walkingGridCell->halfFaceLength; 
                    }
                    else if( walkingGridCell->type == 'w' )
                    {
                        xDistanceModulus = halfFace/2.0;
                        yDistanceModulus = halfFace/2.0;
                    }
              
                }//end while
                
                //__________________________________________________________
                //Sets the variable value at the characteristic foot. 
                if( walkingGridCell->type == 'b' )
                     gridCell->u = walkingGridCell->previousU;
                else if( walkingGridCell->type == 'w' )
                     gridCell->u = 0.0;
                     
            }//end if ( xDistanceModulus > halfFace || yDistanceModulus > halfFace ) )
            
            //__________________________________________________________
            //If the characteristic foot is located in the current cell itself, 
            //sets the foot variable value as the value of corresponding 
            //variable in the current cell.
            else
                gridCell->u = gridCell->previousU;       
                    
                        
        }
                    
        gridCell = gridCell->next;
    }    
}//end characteristicsMethod() function

/*______________________________________________________________________________
                            
                            FUNCTION findCharacteristicFoot()
                            
    Finds the x,y coordinates of the characteristic foot.
     
______________________________________________________________________________*/
void findCharacteristicFoot( Grid *grid, Cell *gridCell, double characteristicFoot[] )
{
    characteristicFoot[0] = gridCell->centerX - grid->velocityX * grid->deltaT; 
    characteristicFoot[1] = gridCell->centerY - grid->velocityY * grid->deltaT;
}//end findCharacteristicFoot() function

/*______________________________________________________________________________
                            
                            FUNCTION findsCharacteristicNeighborCell()
                            
    Finds the next neighbor cell where the characteristic foot passes through,
    jumping through the transition nodes in between. 
    It works by constantly altering the identity of neighborGridCell, as long as 
    it is a transition node.
    If gridCell is a boundary cell, sets values of footVelocityX, footVelocityY 
    or footTemperature, according to the problem being currently solved.
     
______________________________________________________________________________*/
void findsCharacteristicNeighborCell( Cell *walkingGridCell, Cell *neighborGridCell )
{
    /* If neighborGridCell is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new neighborGridCell.
  	   This is repeatedly done until a cell node is found, unless gridCell is
  	   itself a boundary cell. */
    if ( neighborGridCell->level > walkingGridCell->level )
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
}//end findsCharacteristicNeighborCell() function

/*______________________________________________________________________________
                            
                            FUNCTION storePreviousValues()
                            
    Stores previous values.
     
______________________________________________________________________________*/
void storePreviousValues( Grid *grid )
{
    Cell *gridCell;
    
    gridCell = grid->firstGridCell;    
    while( gridCell != 0 )
    {
        gridCell->previousU = gridCell->u;
            
        gridCell = gridCell->next;
    }    
}//end storePreviousValues() function
//end file characteristicMethod.cpp
