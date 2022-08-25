 /*  ========== finitedifference.cpp =========
    Contem funcoes que resolvem as tensoes XX, XY, YY, e fluxo de calor QX, QY
*/

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>
#include <iostream>

#include "constants.h"

/*______________________________________________________________________________
                            
                        FUNCTIONs Tension XX, XY, YY

      Calculated by finite difference. the parameter boundary indicates if 
the tension is calculated at the boundaries (of the grid) or at the 
cell nodes.
______________________________________________________________________________*/

// ----------------------------- TAU_XX -------------------------------------
double tauXX( Cell *gridCell, char boundary ) 
{
    Cell *cellEast, *cellWest,
         *cellNorth, *cellSouth;

    double deltaX = 1.0;
    double deltaY = 1.0;

    double du_dx, dv_dy;
    double lambda = -(2.0/3.0)*(gridCell->miIterAnterior);

    cellEast = gridCell->east;
    cellWest = gridCell->west;
    cellNorth = gridCell->north;
    cellSouth = gridCell->south;

    // Buscar vizinho valido do leste
    /* If cellEast is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellEast.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellEast->level > gridCell->level )
    	while( ( cellEast->type == 'w' ) && 
               ( cellEast->singleConnector != 0 ) 
             )
    	{
        	cellEast = cellEast->doubleConnector1;
    	}   	
  	else
    	while( ( cellEast->type == 'w' ) && 
               ( cellEast->singleConnector != 0 ) 
             )
    	{
    		cellEast = cellEast->singleConnector;
   		}

    // Buscar vizinho valido do oeste
    /* If cellWest is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellWest.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellWest->level > gridCell->level )
    	while( ( cellWest->type == 'w' ) && 
               ( cellWest->singleConnector != 0 ) 
             )
    	{
        	cellWest = cellWest->doubleConnector1;
    	}   	
  	else
    	while( ( cellWest->type == 'w' ) && 
               ( cellWest->singleConnector != 0 ) 
             )
    	{
    		cellWest = cellWest->singleConnector;
   		}

    // Buscar vizinho valido do norte
    /* If cellNorth is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellNorth.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellNorth->level > gridCell->level )
    	while( ( cellNorth->type == 'w' ) && 
               ( cellNorth->singleConnector != 0 ) 
             )
    	{
        	cellNorth = cellNorth->doubleConnector1;
    	}   	
  	else
    	while( ( cellNorth->type == 'w' ) && 
               ( cellNorth->singleConnector != 0 ) 
             )
    	{
    		cellNorth = cellNorth->singleConnector;
   		}

    // Buscar vizinho valido do sul
    /* If cellSouth is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellSouth.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellSouth->level > gridCell->level )
    	while( ( cellSouth->type == 'w' ) && 
               ( cellSouth->singleConnector != 0 ) 
             )
    	{
        	cellSouth = cellSouth->doubleConnector1;
    	}   	
  	else
    	while( ( cellSouth->type == 'w' ) && 
               ( cellSouth->singleConnector != 0 ) 
             )
    	{
    		cellSouth = cellSouth->singleConnector;
   		}

    // Delta x total = celula->facelength + oeste->halfFaceLength + leste->halfFaceLength
    deltaX = gridCell->faceLength + 
      ( ((cellWest->type == 'w')||(cellWest->active == false))? 
            0 
            : 
            cellWest->halfFaceLength
      )   
      + 
      ( ((cellEast->type == 'w' )||(cellEast->active == false))? 
            0 
            : 
            cellEast->halfFaceLength
      );

    // Delta y total = celula->facelength + norte->halfFaceLength + sul->halfFaceLength
    deltaY = gridCell->faceLength + 
      ( ((cellNorth->type == 'w' )||(cellNorth->active == false))? 
            0 
            :
            cellNorth->halfFaceLength
      ) 
      + 
      ( ((cellSouth->type == 'w' )||(cellSouth->active == false))? 
            0 
            : 
            cellSouth->halfFaceLength
      );

    // Calcula derivadas parciais
    if(boundary == '0'){ // TAU_XX para celulas internas
        du_dx = (gridCell->eastU - gridCell->westU) / deltaX;
        dv_dy = (gridCell->northV - gridCell->southV) / deltaY;
    }

    else{ // TAU_XX para fronteiras
        if(boundary == 's'){ // SUL ------------------------
            if( (cellWest->type == 'b')  // Se os dois laterais vizinhos forem internos
            && (cellWest->active == true)
            && (cellEast->type == 'b') 
            && (cellEast->active == true) )
            {
                du_dx = (cellEast->southU - cellWest->southU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->vIterAnterior - gridCell->southV) / deltaY;              
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellWest->type == 'w') || (cellWest->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                du_dx = (cellEast->southU - gridCell->southU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->vIterAnterior - gridCell->southV) / deltaY;
            }        
               // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellEast->type == 'w') || (cellEast->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                du_dx = (gridCell->southU - cellWest->southU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->vIterAnterior - gridCell->southV) / deltaY;
            }
        }

        if(boundary == 'n'){ // NORTE ------------------------
            if( (cellWest->type == 'b')  // Se os dois laterais vizinhos forem internos
            && (cellWest->active == true)
            && (cellEast->type == 'b') 
            && (cellEast->active == true) )
            {
                du_dx = (cellEast->northU - cellWest->northU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->northV - gridCell->vIterAnterior) / deltaY;              
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellWest->type == 'w') || (cellWest->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                du_dx = (cellEast->northU - gridCell->northU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->northV - gridCell->vIterAnterior) / deltaY;
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellEast->type == 'w') || (cellEast->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                du_dx = (gridCell->northU - cellWest->northU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->northV - gridCell->vIterAnterior) / deltaY;
            }
        }

        if(boundary == 'w'){ // OESTE ------------------------
            if( (cellNorth->type == 'b')  // Se os dois vizinhos verticais forem internos
            && (cellNorth->active == true)
            && (cellSouth->type == 'b') 
            && (cellSouth->active == true) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->uIterAnterior - gridCell->westU) / deltaX;

                dv_dy = (cellNorth->westV - cellSouth->westV) / deltaY;              
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->uIterAnterior - gridCell->westU) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                dv_dy = (gridCell->westV - cellSouth->westV) / deltaY;
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->uIterAnterior - gridCell->westU) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                dv_dy = (cellNorth->westV - gridCell->westV) / deltaY;
            }
        }

        if(boundary == 'e'){ // LESTE ------------------------
            if( (cellNorth->type == 'b')  // Se os dois vizinhos verticais forem internos
            && (cellNorth->active == true)
            && (cellSouth->type == 'b') 
            && (cellSouth->active == true) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->eastU - gridCell->uIterAnterior) / deltaX;

                dv_dy = (cellNorth->eastV - cellSouth->eastV) / deltaY;              
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->eastU - gridCell->uIterAnterior) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                dv_dy = (gridCell->eastV - cellSouth->eastV) / deltaY;
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->eastU - gridCell->uIterAnterior) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                dv_dy = (cellNorth->eastV - gridCell->eastV) / deltaY;
            }
        }
    }
       
    return lambda*(du_dx + dv_dy) + 2.0*(gridCell->miIterAnterior)*du_dx;  
}

// ----------------------------- TAU_XY -------------------------------------
double tauXY( Cell *gridCell, char boundary ) 
{
    Cell *cellEast, *cellWest,
         *cellNorth, *cellSouth;

    double deltaX = 1.0;
    double deltaY = 1.0;
    double du_dy, dv_dx;

    cellEast = gridCell->east;
    cellWest = gridCell->west;
    cellNorth = gridCell->north;
    cellSouth = gridCell->south;

    // Buscar vizinho valido do leste
    /* If cellEast is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellEast.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellEast->level > gridCell->level )
    	while( ( cellEast->type == 'w' ) && 
               ( cellEast->singleConnector != 0 ) 
             )
    	{
        	cellEast = cellEast->doubleConnector1;
    	}   	
  	else
    	while( ( cellEast->type == 'w' ) && 
               ( cellEast->singleConnector != 0 ) 
             )
    	{
    		cellEast = cellEast->singleConnector;
   		}

    // Buscar vizinho valido do oeste
    /* If cellWest is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellWest.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellWest->level > gridCell->level )
    	while( ( cellWest->type == 'w' ) && 
               ( cellWest->singleConnector != 0 ) 
             )
    	{
        	cellWest = cellWest->doubleConnector1;
    	}   	
  	else
    	while( ( cellWest->type == 'w' ) && 
               ( cellWest->singleConnector != 0 ) 
             )
    	{
    		cellWest = cellWest->singleConnector;
   		}

    // Buscar vizinho valido do norte
    /* If cellNorth is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellNorth.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellNorth->level > gridCell->level )
    	while( ( cellNorth->type == 'w' ) && 
               ( cellNorth->singleConnector != 0 ) 
             )
    	{
        	cellNorth = cellNorth->doubleConnector1;
    	}   	
  	else
    	while( ( cellNorth->type == 'w' ) && 
               ( cellNorth->singleConnector != 0 ) 
             )
    	{
    		cellNorth = cellNorth->singleConnector;
   		}

    // Buscar vizinho valido do sul
    /* If cellSouth is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellSouth.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellSouth->level > gridCell->level )
    	while( ( cellSouth->type == 'w' ) && 
               ( cellSouth->singleConnector != 0 ) 
             )
    	{
        	cellSouth = cellSouth->doubleConnector1;
    	}   	
  	else
    	while( ( cellSouth->type == 'w' ) && 
               ( cellSouth->singleConnector != 0 ) 
             )
    	{
    		cellSouth = cellSouth->singleConnector;
   		}

    // Delta x total = celula->facelength + oeste->halfFaceLength + leste->halfFaceLength
    deltaX = gridCell->faceLength + 
      ( ((cellWest->type == 'w')||(cellWest->active == false))? 
            0 
            : 
            cellWest->halfFaceLength
      )   
      + 
      ( ((cellEast->type == 'w' )||(cellEast->active == false))? 
            0 
            : 
            cellEast->halfFaceLength
      );

    // Delta y total = celula->facelength + norte->halfFaceLength + sul->halfFaceLength
    deltaY = gridCell->faceLength + 
      ( ((cellNorth->type == 'w' )||(cellNorth->active == false))? 
            0 
            :
            cellNorth->halfFaceLength
      ) 
      + 
      ( ((cellSouth->type == 'w' )||(cellSouth->active == false))? 
            0 
            : 
            cellSouth->halfFaceLength
      );

    // Calcula derivadas parciais
    if(boundary == '0'){ // TAU_XY para celulas internas
        du_dy = (gridCell->northU - gridCell->southU) / deltaY;
        dv_dx = (gridCell->eastV - gridCell->westV) / deltaX;
    }

    else{ // TAU_XY para fronteiras
        if(boundary == 's'){ // SUL ------------------------
            if( (cellWest->type == 'b')  // Se os dois laterais vizinhos forem internos
            && (cellWest->active == true)
            && (cellEast->type == 'b') 
            && (cellEast->active == true) )
            {
                dv_dx = (cellEast->southV - cellWest->southV) / deltaX;

                deltaY = gridCell->halfFaceLength;
                du_dy = (gridCell->uIterAnterior - gridCell->southU) / deltaY;              
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellWest->type == 'w') || (cellWest->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                dv_dx = (cellEast->southV - gridCell->southV) / deltaX;

                deltaY = gridCell->halfFaceLength;
                du_dy = (gridCell->uIterAnterior - gridCell->southU) / deltaY;
            }        
               // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellEast->type == 'w') || (cellEast->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                dv_dx = (gridCell->southV - cellWest->southV) / deltaX;

                deltaY = gridCell->halfFaceLength;
                du_dy = (gridCell->uIterAnterior - gridCell->southU) / deltaY;
            }
        }

        if(boundary == 'n'){ // NORTE ------------------------
            if( (cellWest->type == 'b')  // Se os dois laterais vizinhos forem internos
            && (cellWest->active == true)
            && (cellEast->type == 'b') 
            && (cellEast->active == true) )
            {
                dv_dx = (cellEast->northV - cellWest->northV) / deltaX;

                deltaY = gridCell->halfFaceLength;
                du_dy = (gridCell->northU - gridCell->uIterAnterior) / deltaY;              
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellWest->type == 'w') || (cellWest->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                dv_dx = (cellEast->northV - gridCell->northV) / deltaX;

                deltaY = gridCell->halfFaceLength;
                du_dy = (gridCell->northU - gridCell->uIterAnterior) / deltaY;
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellEast->type == 'w') || (cellEast->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                dv_dx = (gridCell->northV - cellWest->northV) / deltaX;

                deltaY = gridCell->halfFaceLength;
                du_dy = (gridCell->northU - gridCell->uIterAnterior) / deltaY;
            }
        }

        if(boundary == 'w'){ // OESTE ------------------------
            if( (cellNorth->type == 'b')  // Se os dois vizinhos verticais forem internos
            && (cellNorth->active == true)
            && (cellSouth->type == 'b') 
            && (cellSouth->active == true) )
            {
                deltaX = gridCell->halfFaceLength;
                dv_dx = (gridCell->vIterAnterior - gridCell->westV) / deltaX;

                du_dy = (cellNorth->westU - cellSouth->westU) / deltaY;              
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                dv_dx = (gridCell->vIterAnterior - gridCell->westV) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                du_dy = (gridCell->westU - cellSouth->westU) / deltaY;
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                dv_dx = (gridCell->vIterAnterior - gridCell->westV) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                du_dy = (cellNorth->westU - gridCell->westU) / deltaY;
            }
        }

        if(boundary == 'e'){ // LESTE ------------------------
            if( (cellNorth->type == 'b')  // Se os dois vizinhos verticais forem internos
            && (cellNorth->active == true)
            && (cellSouth->type == 'b') 
            && (cellSouth->active == true) )
            {
                deltaX = gridCell->halfFaceLength;
                dv_dx = (gridCell->eastV - gridCell->vIterAnterior) / deltaX;

                du_dy = (cellNorth->eastU - cellSouth->eastU) / deltaY;              
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                dv_dx = (gridCell->eastV - gridCell->vIterAnterior) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                du_dy = (gridCell->eastU - cellSouth->eastU) / deltaY;
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                dv_dx = (gridCell->eastV - gridCell->vIterAnterior) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                du_dy = (cellNorth->eastU - gridCell->eastU) / deltaY;
            }
        }
    }


    return (gridCell->miIterAnterior)*(du_dy + dv_dx);  
}

// ----------------------------- TAU_YY -------------------------------------
double tauYY( Cell *gridCell, char boundary ) 
{
    Cell *cellEast, *cellWest,
         *cellNorth, *cellSouth;

    double deltaX = 1.0;
    double deltaY = 1.0;

    double du_dx, dv_dy;
    double lambda = -(2.0/3.0)*(gridCell->miIterAnterior);

    cellEast = gridCell->east;
    cellWest = gridCell->west;
    cellNorth = gridCell->north;
    cellSouth = gridCell->south;

    // Buscar vizinho valido do leste
    /* If cellEast is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellEast.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellEast->level > gridCell->level )
    	while( ( cellEast->type == 'w' ) && 
               ( cellEast->singleConnector != 0 ) 
             )
    	{
        	cellEast = cellEast->doubleConnector1;
    	}   	
  	else
    	while( ( cellEast->type == 'w' ) && 
               ( cellEast->singleConnector != 0 ) 
             )
    	{
    		cellEast = cellEast->singleConnector;
   		}

    // Buscar vizinho valido do oeste
    /* If cellWest is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellWest.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellWest->level > gridCell->level )
    	while( ( cellWest->type == 'w' ) && 
               ( cellWest->singleConnector != 0 ) 
             )
    	{
        	cellWest = cellWest->doubleConnector1;
    	}   	
  	else
    	while( ( cellWest->type == 'w' ) && 
               ( cellWest->singleConnector != 0 ) 
             )
    	{
    		cellWest = cellWest->singleConnector;
   		}

    // Buscar vizinho valido do norte
    /* If cellNorth is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellNorth.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellNorth->level > gridCell->level )
    	while( ( cellNorth->type == 'w' ) && 
               ( cellNorth->singleConnector != 0 ) 
             )
    	{
        	cellNorth = cellNorth->doubleConnector1;
    	}   	
  	else
    	while( ( cellNorth->type == 'w' ) && 
               ( cellNorth->singleConnector != 0 ) 
             )
    	{
    		cellNorth = cellNorth->singleConnector;
   		}

    // Buscar vizinho valido do sul
    /* If cellSouth is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellSouth.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellSouth->level > gridCell->level )
    	while( ( cellSouth->type == 'w' ) && 
               ( cellSouth->singleConnector != 0 ) 
             )
    	{
        	cellSouth = cellSouth->doubleConnector1;
    	}   	
  	else
    	while( ( cellSouth->type == 'w' ) && 
               ( cellSouth->singleConnector != 0 ) 
             )
    	{
    		cellSouth = cellSouth->singleConnector;
   		}

    // Delta x total = celula->facelength + oeste->halfFaceLength + leste->halfFaceLength
    deltaX = gridCell->faceLength + 
      ( ((cellWest->type == 'w')||(cellWest->active == false))? 
            0 
            : 
            cellWest->halfFaceLength
      )   
      + 
      ( ((cellEast->type == 'w' )||(cellEast->active == false))? 
            0 
            : 
            cellEast->halfFaceLength
      );

    // Delta y total = celula->facelength + norte->halfFaceLength + sul->halfFaceLength
    deltaY = gridCell->faceLength + 
      ( ((cellNorth->type == 'w' )||(cellNorth->active == false))? 
            0 
            :
            cellNorth->halfFaceLength
      ) 
      + 
      ( ((cellSouth->type == 'w' )||(cellSouth->active == false))? 
            0 
            : 
            cellSouth->halfFaceLength
      );

    // Calcula derivadas parciais
    if(boundary == '0'){ // TAU_YY para celulas internas
        du_dx = (gridCell->eastU - gridCell->westU) / deltaX;
        dv_dy = (gridCell->northV - gridCell->southV) / deltaY;
    }

    else{ // TAU_YY para fronteiras
        if(boundary == 's'){ // SUL ------------------------
            if( (cellWest->type == 'b')  // Se os dois laterais vizinhos forem internos
            && (cellWest->active == true)
            && (cellEast->type == 'b') 
            && (cellEast->active == true) )
            {
                du_dx = (cellEast->southU - cellWest->southU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->vIterAnterior - gridCell->southV) / deltaY;              
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellWest->type == 'w') || (cellWest->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                du_dx = (cellEast->southU - gridCell->southU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->vIterAnterior - gridCell->southV) / deltaY;
            }        
               // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellEast->type == 'w') || (cellEast->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                du_dx = (gridCell->southU - cellWest->southU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->vIterAnterior - gridCell->southV) / deltaY;
            }
        }

        if(boundary == 'n'){ // NORTE ------------------------
            if( (cellWest->type == 'b')  // Se os dois laterais vizinhos forem internos
            && (cellWest->active == true)
            && (cellEast->type == 'b') 
            && (cellEast->active == true) )
            {
                du_dx = (cellEast->northU - cellWest->northU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->northV - gridCell->vIterAnterior) / deltaY;              
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellWest->type == 'w') || (cellWest->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                du_dx = (cellEast->northU - gridCell->northU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->northV - gridCell->vIterAnterior) / deltaY;
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellEast->type == 'w') || (cellEast->active == false) )
            {
                deltaX = deltaX - gridCell->halfFaceLength;
                du_dx = (gridCell->northU - cellWest->northU) / deltaX;

                deltaY = gridCell->halfFaceLength;
                dv_dy = (gridCell->northV - gridCell->vIterAnterior) / deltaY;
            }
        }

        if(boundary == 'w'){ // OESTE ------------------------
            if( (cellNorth->type == 'b')  // Se os dois vizinhos verticais forem internos
            && (cellNorth->active == true)
            && (cellSouth->type == 'b') 
            && (cellSouth->active == true) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->uIterAnterior - gridCell->westU) / deltaX;

                dv_dy = (cellNorth->westV - cellSouth->westV) / deltaY;              
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->uIterAnterior - gridCell->westU) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                dv_dy = (gridCell->westV - cellSouth->westV) / deltaY;
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->uIterAnterior - gridCell->westU) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                dv_dy = (cellNorth->westV - gridCell->westV) / deltaY;
            }
        }

        if(boundary == 'e'){ // LESTE ------------------------
            if( (cellNorth->type == 'b')  // Se os dois vizinhos verticais forem internos
            && (cellNorth->active == true)
            && (cellSouth->type == 'b') 
            && (cellSouth->active == true) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->eastU - gridCell->uIterAnterior) / deltaX;

                dv_dy = (cellNorth->eastV - cellSouth->eastV) / deltaY;              
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->eastU - gridCell->uIterAnterior) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                dv_dy = (gridCell->eastV - cellSouth->eastV) / deltaY;
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
            {
                deltaX = gridCell->halfFaceLength;
                du_dx = (gridCell->eastU - gridCell->uIterAnterior) / deltaX;

                deltaY = deltaY - gridCell->halfFaceLength;
                dv_dy = (cellNorth->eastV - gridCell->eastV) / deltaY;
            }
        }
    }
    

    return lambda*(du_dx + dv_dy) + 2.0*(gridCell->miIterAnterior)*dv_dy;  
}

// -------------------------------- qX ---------------------------------------
double qX( Cell *gridCell, char boundary ) 
{
    Cell *cellEast, *cellWest;

    double deltaX = 1.0;
    double dT_dx;

    cellEast = gridCell->east;
    cellWest = gridCell->west;

    // Buscar vizinho valido do leste
    /* If cellEast is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellEast.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellEast->level > gridCell->level )
    	while( ( cellEast->type == 'w' ) && 
               ( cellEast->singleConnector != 0 ) 
             )
    	{
        	cellEast = cellEast->doubleConnector1;
    	}   	
  	else
    	while( ( cellEast->type == 'w' ) && 
               ( cellEast->singleConnector != 0 ) 
             )
    	{
    		cellEast = cellEast->singleConnector;
   		}

    // Buscar vizinho valido do oeste
    /* If cellWest is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellWest.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellWest->level > gridCell->level )
    	while( ( cellWest->type == 'w' ) && 
               ( cellWest->singleConnector != 0 ) 
             )
    	{
        	cellWest = cellWest->doubleConnector1;
    	}   	
  	else
    	while( ( cellWest->type == 'w' ) && 
               ( cellWest->singleConnector != 0 ) 
             )
    	{
    		cellWest = cellWest->singleConnector;
   		}

    // Delta x total = celula->facelength + oeste->halfFaceLength + leste->halfFaceLength
    deltaX = gridCell->faceLength + 
      ( ((cellWest->type == 'w')||(cellWest->active == false))? 
            0 
            : 
            cellWest->halfFaceLength
      )   
      + 
      ( ((cellEast->type == 'w' )||(cellEast->active == false))? 
            0 
            : 
            cellEast->halfFaceLength
      );

    // Calcula derivadas parciais
    if(boundary == '0'){ // qX para celulas internas
        // Diferencas centradas
        dT_dx = (gridCell->eastT - gridCell->westT) / deltaX;
    }

    else{ // qX para fronteiras
        if(boundary == 's'){ // SUL ------------------------
            if( (cellWest->type == 'b')  // Se os dois laterais vizinhos forem internos
            && (cellWest->active == true)
            && (cellEast->type == 'b') 
            && (cellEast->active == true) )
            {
                // Diferencas centradas
                dT_dx = (cellEast->southT - cellWest->southT) / deltaX;                                        
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellWest->type == 'w') || (cellWest->active == false) )
            {
                // Diferencas avancadas
                deltaX = deltaX - gridCell->halfFaceLength;
                dT_dx = (cellEast->southT - gridCell->southT) / deltaX;
            }        
               // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellEast->type == 'w') || (cellEast->active == false) )
            {
                // Diferencas atrasadas
                deltaX = deltaX - gridCell->halfFaceLength;
                dT_dx = (gridCell->southT - cellWest->southT) / deltaX;
            }
        }

        if(boundary == 'n'){ // NORTE ------------------------
            if( (cellWest->type == 'b')  // Se os dois laterais vizinhos forem internos
            && (cellWest->active == true)
            && (cellEast->type == 'b') 
            && (cellEast->active == true) )
            {   // Diferencas centradas            
                dT_dx = (cellEast->northT - cellWest->northT) / deltaX; 
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellWest->type == 'w') || (cellWest->active == false) )
            {
                // Diferencas avancadas
                deltaX = deltaX - gridCell->halfFaceLength;
                dT_dx = (cellEast->northT - gridCell->northT) / deltaX;
            }
                // Se um dos vizinhos laterais e' fronteira tambem
            if( (cellEast->type == 'w') || (cellEast->active == false) )
            {
                // Diferencas atrasadas
                deltaX = deltaX - gridCell->halfFaceLength;
                dT_dx = (gridCell->northT - cellWest->northT) / deltaX;
            }
        }

        if(boundary == 'w'){ // OESTE ------------------------
            // Diferencas atrasadas
            deltaX = gridCell->halfFaceLength;
            dT_dx = (gridCell->TIterAnterior - gridCell->westT) / deltaX;                         
        }

        if(boundary == 'e'){ // LESTE ------------------------
            // Diferencas avancadas
            deltaX = gridCell->halfFaceLength;
            dT_dx = (gridCell->eastT - gridCell->TIterAnterior) / deltaX;              
        }
    }
       
    return -(gridCell->kIterAnterior)*dT_dx;  
}


// --------------------------------- qY ---------------------------------------
double qY( Cell *gridCell, char boundary )         
{
    Cell *cellNorth, *cellSouth;

    double deltaY = 1.0;
    double dT_dy;

    cellNorth = gridCell->north;
    cellSouth = gridCell->south;

    // Buscar vizinho valido do norte
    /* If cellNorth is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellNorth.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellNorth->level > gridCell->level )
    	while( ( cellNorth->type == 'w' ) && 
               ( cellNorth->singleConnector != 0 ) 
             )
    	{
        	cellNorth = cellNorth->doubleConnector1;
    	}   	
  	else
    	while( ( cellNorth->type == 'w' ) && 
               ( cellNorth->singleConnector != 0 ) 
             )
    	{
    		cellNorth = cellNorth->singleConnector;
   		}

    // Buscar vizinho valido do sul
    /* If cellSouth is a transition node, considers the cell to which its
       doubleConnector1 points to, which will be the new cellSouth.
  	   This is repeatedly done until a cell node is found. */
  	if ( cellSouth->level > gridCell->level )
    	while( ( cellSouth->type == 'w' ) && 
               ( cellSouth->singleConnector != 0 ) 
             )
    	{
        	cellSouth = cellSouth->doubleConnector1;
    	}   	
  	else
    	while( ( cellSouth->type == 'w' ) && 
               ( cellSouth->singleConnector != 0 ) 
             )
    	{
    		cellSouth = cellSouth->singleConnector;
   		}

    // Delta y total = celula->facelength + norte->halfFaceLength + sul->halfFaceLength
    deltaY = gridCell->faceLength + 
      ( ((cellNorth->type == 'w' )||(cellNorth->active == false))? 
            0 
            :
            cellNorth->halfFaceLength
      ) 
      + 
      ( ((cellSouth->type == 'w' )||(cellSouth->active == false))? 
            0 
            : 
            cellSouth->halfFaceLength
      );

    // Calcula derivadas parciais
    if(boundary == '0'){ // qY para celulas internas
        dT_dy = (gridCell->northT - gridCell->southT) / deltaY;
    }

    else{ // qY para fronteiras
        if(boundary == 's'){ // SUL ------------------------
            deltaY = gridCell->halfFaceLength;
            dT_dy = (gridCell->TIterAnterior - gridCell->southT) / deltaY;                  
        }

        if(boundary == 'n'){ // NORTE ------------------------
            deltaY = gridCell->halfFaceLength;
            dT_dy = (gridCell->northT - gridCell->TIterAnterior) / deltaY;              
        }

        if(boundary == 'w'){ // OESTE ------------------------
            if( (cellNorth->type == 'b')  // Se os dois vizinhos verticais forem internos
            && (cellNorth->active == true)
            && (cellSouth->type == 'b') 
            && (cellSouth->active == true) )
            {
                dT_dy = (cellNorth->westT - cellSouth->westT) / deltaY;              
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
            {
                deltaY = deltaY - gridCell->halfFaceLength;
                dT_dy = (gridCell->westT - cellSouth->westT) / deltaY;
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
            {
                deltaY = deltaY - gridCell->halfFaceLength;
                dT_dy = (cellNorth->westT - gridCell->westT) / deltaY;
            }
        }

        if(boundary == 'e'){ // LESTE ------------------------
            if( (cellNorth->type == 'b')  // Se os dois vizinhos verticais forem internos
            && (cellNorth->active == true)
            && (cellSouth->type == 'b') 
            && (cellSouth->active == true) )
            {
                dT_dy = (cellNorth->eastT - cellSouth->eastT) / deltaY;              
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellNorth->type == 'w') || (cellNorth->active == false) )
            {
                deltaY = deltaY - gridCell->halfFaceLength;
                dT_dy = (gridCell->eastT - cellSouth->eastT) / deltaY;
            }
                // Se um dos vizinhos verticais e' fronteira tambem
            if( (cellSouth->type == 'w') || (cellSouth->active == false) )
            {
                deltaY = deltaY - gridCell->halfFaceLength;
                dT_dy = (cellNorth->eastT - gridCell->eastT) / deltaY;
            }
        }
    }
       
    return -(gridCell->kIterAnterior)*dT_dy; 
}
