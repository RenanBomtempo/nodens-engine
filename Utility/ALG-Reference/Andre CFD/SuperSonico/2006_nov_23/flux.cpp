//flux.cpp

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>

#include <iostream>

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

/*______________________________________________________________________________
    
                         FUNCTION computeExpressionFlux()
    
    Computes expression at a cell face between a neighbor cell.
	 
    Variable_at_face = (Variable_at_cell + weight * (Variable_at_neighbor))
                                        / ( 1 + weight )
    
    where 'weight' is 2 power (refinement difference between cells).    
______________________________________________________________________________*/
double computeExpressionFlux( Cell *gridCell, Cell *neighborGridCell, 
                              int expression, char direction )
{
    int deltaRefinamento = 0;
    double peso = 1; // Para a media ponderada na interpolacao

    // Procurar vizinho valido
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

    // Para vizinhos do interior da malha
    if( ( neighborGridCell->type == 'b' ) && ( neighborGridCell->active == true ) )
    {
        // Buscar diferenca: ref(vizinho) - ref(atual)
        // E computar peso para a media ponderada
        deltaRefinamento = neighborGridCell->level - gridCell->level;

        if(deltaRefinamento > 0){
            while(deltaRefinamento > 0){
                peso *= 2.0;
                deltaRefinamento--;    
            }
        }
        if(deltaRefinamento < 0){
            while(deltaRefinamento < 0){
                peso /= 2.0;
                deltaRefinamento++;
            }
        }
        // Computar expressao por media ponderada da diferenca acima
        switch(direction)
        {             /* ----------------------------------------------- */
            case 'w': /* ------------------ OESTE ---------------------- */     
                if(expression == 1 /* rho u */)
                    return ((gridCell->rhoIterAnterior)*(gridCell->uIterAnterior)
                                 + 
                             peso*(gridCell->westRho)*(gridCell->westU)
                           ) / (1.0 + peso);                

                if(expression == 2 /*rho u^2*/ )
                    return ((gridCell->rhoIterAnterior)
                            *(gridCell->uIterAnterior)*(gridCell->uIterAnterior)
                                 + 
                             peso*(gridCell->westRho)
                            *(gridCell->westU)*(gridCell->westU)
                           ) / (1.0 + peso);

                if(expression == 3 /* p */)
                    return ((gridCell->pIterAnterior)
                                 + 
                             peso*(gridCell->westP)
                           ) / (1.0 + peso);

                if(expression == 4 /* tau_xx */)
                    return ( tauXX(gridCell,'0')
                                 + 
                             peso*tauXX(gridCell->west,'0')
                           ) / (1.0 + peso);

                if(expression == 6 /* rho u v */ )
                    return ((gridCell->rhoIterAnterior)
                            *(gridCell->uIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->westRho)
                            *(gridCell->westU)*(gridCell->westV)
                           ) / (1.0 + peso);

                if(expression == 7 /* tau_xy */ )
                    return ( tauXY(gridCell,'0')
                                 + 
                             peso*tauXY(gridCell->west,'0')
                           ) / (1.0 + peso);

                if(expression == 9 /* Et u */ )
                    return( (gridCell->rhoIterAnterior)
                            *(gridCell->uIterAnterior)
                            *(
                                (gridCell->eIterAnterior) 
                                    +
                                (
                                    (gridCell->uIterAnterior)   
                                   *(gridCell->uIterAnterior)
                                        +
                                    (gridCell->vIterAnterior)
                                   *(gridCell->vIterAnterior)
                                )/2.0
                            )
                                 + 
                        peso*(gridCell->westRho)
                            *(gridCell->westU)
                            *(
                                (gridCell->westE) 
                                    +
                                (
                                    (gridCell->westU)   
                                   *(gridCell->westU)
                                        +
                                    (gridCell->westV)
                                   *(gridCell->westV)
                                )/2.0
                             )
                           ) / (1.0 + peso);

                if(expression == 10 /* p u */)
                    return ((gridCell->pIterAnterior)*(gridCell->uIterAnterior)
                                 + 
                             peso*(gridCell->westP)*(gridCell->westU)
                           ) / (1.0 + peso);

                if(expression == 11 /* u tau_xx */)
                    return ( (gridCell->uIterAnterior)
                             *tauXX(gridCell,'0')
                                 + 
                             peso*(gridCell->westU)
                             *tauXX(gridCell->west,'0')
                           ) / (1.0 + peso);

                if(expression == 12 /* v tau_xy */)
                    return ( (gridCell->vIterAnterior)
                             *tauXY(gridCell,'0')
                                 + 
                             peso*(gridCell->westV)
                             *tauXY(gridCell->west,'0')
                           ) / (1.0 + peso);

                if(expression == 15 /* qX */ )
                    return ( qX(gridCell,'0')
                                 + 
                             peso*qX(gridCell->west,'0')
                           ) / (1.0 + peso);
                           
            break;
                      /* ----------------------------------------------- */
            case 'e': /* ------------------ LESTE ---------------------- */
                if(expression == 1 /* rho u */)
                    return ((gridCell->rhoIterAnterior)*(gridCell->uIterAnterior)
                                 + 
                             peso*(gridCell->eastRho)*(gridCell->eastU)
                           ) / (1.0 + peso);

                if(expression == 2 /* rho u^2 */)
                    return ((gridCell->rhoIterAnterior)
                            *(gridCell->uIterAnterior)*(gridCell->uIterAnterior)
                                 + 
                             peso*(gridCell->eastRho)
                            *(gridCell->eastU)*(gridCell->eastU)
                           ) / (1.0 + peso);

                if(expression == 3 /* p */)
                    return ((gridCell->pIterAnterior)         
                                 + 
                             peso*(gridCell->eastP)
                           ) / (1.0 + peso); 

                if(expression == 4 /* tau_xx */)
                    return ( tauXX(gridCell,'0')
                                 + 
                             peso*tauXX(gridCell->east,'0')
                           ) / (1.0 + peso);

                if(expression == 6 /* rho u v */ )
                    return ((gridCell->rhoIterAnterior)
                            *(gridCell->uIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->eastRho)
                            *(gridCell->eastU)*(gridCell->eastV)
                           ) / (1.0 + peso);

                if(expression == 7 /* tau_xy */ )
                    return ( tauXY(gridCell,'0')
                                 + 
                             peso*tauXY(gridCell->east,'0')
                           ) / (1.0 + peso);

                if(expression == 9 /* Et u */ )
                    return( (gridCell->rhoIterAnterior)
                            *(gridCell->uIterAnterior)
                            *(
                                (gridCell->eIterAnterior) 
                                    +
                                (
                                    (gridCell->uIterAnterior)   
                                   *(gridCell->uIterAnterior)
                                        +
                                    (gridCell->vIterAnterior)
                                   *(gridCell->vIterAnterior)
                                )/2.0
                            )
                                 + 
                        peso*(gridCell->eastRho)
                            *(gridCell->eastU)
                            *(
                                (gridCell->eastE) 
                                    +
                                (
                                    (gridCell->eastU)   
                                   *(gridCell->eastU)
                                        +
                                    (gridCell->eastV)
                                   *(gridCell->eastV)
                                )/2.0
                             )
                           ) / (1.0 + peso);

                if(expression == 10 /* p u */)
                    return ((gridCell->pIterAnterior)*(gridCell->uIterAnterior)
                                 + 
                             peso*(gridCell->eastP)*(gridCell->eastU)
                           ) / (1.0 + peso);

                if(expression == 11 /* u tau_xx */)
                    return ( (gridCell->uIterAnterior)
                             *tauXX(gridCell,'0')
                                 + 
                             peso*(gridCell->eastU)
                             *tauXX(gridCell->east,'0')
                           ) / (1.0 + peso);

                if(expression == 12 /* v tau_xy */)
                    return ( (gridCell->vIterAnterior)
                             *tauXY(gridCell,'0')
                                 + 
                             peso*(gridCell->eastV)
                             *tauXY(gridCell->east,'0')
                           ) / (1.0 + peso);

                if(expression == 15 /* qX */ )
                    return ( qX(gridCell,'0')
                                 + 
                             peso*qX(gridCell->east,'0')
                           ) / (1.0 + peso);

            break;
                       /* ----------------------------------------------- */
            case 'n':  /* ------------------ NORTE ---------------------- */
                if(expression == 5 /* rho v */ )
                    return ((gridCell->rhoIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->northRho)*(gridCell->northV)
                           ) / (1.0 + peso);

                if(expression == 6 /* rho u v */ )
                    return ((gridCell->rhoIterAnterior)
                            *(gridCell->uIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->northRho)
                            *(gridCell->northU)*(gridCell->northV)
                           ) / (1.0 + peso);

                if(expression == 7 /* tau_xy */ )
                    return ( tauXY(gridCell,'0')
                                 + 
                             peso*tauXY(gridCell->north,'0')
                           ) / (1.0 + peso);

                if(expression == 3 /* p */)
                    return ((gridCell->pIterAnterior)         
                                 + 
                             peso*(gridCell->northP)
                           ) / (1.0 + peso);

                if(expression == 8 /* rho v^2 */ )
                    return ((gridCell->rhoIterAnterior)
                            *(gridCell->vIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->northRho)
                            *(gridCell->northV)*(gridCell->northV)
                           ) / (1.0 + peso);

                if(expression == 13 /* u tau_xy */)
                    return ( (gridCell->uIterAnterior)
                             *tauXY(gridCell,'0')
                                 + 
                             peso*(gridCell->northU)
                             *tauXY(gridCell->north,'0')
                           ) / (1.0 + peso);

                if(expression == 14 /* v tau_yy */)
                    return ( (gridCell->vIterAnterior)
                             *tauYY(gridCell,'0')
                                 + 
                             peso*(gridCell->northV)
                             *tauYY(gridCell->north,'0')
                           ) / (1.0 + peso);

                if(expression == 16 /* qY */ )
                    return ( qY(gridCell,'0')
                                 + 
                             peso*qY(gridCell->north,'0')
                           ) / (1.0 + peso);

                if(expression == 17 /* Et v */ )
                    return( (gridCell->rhoIterAnterior)
                            *(gridCell->vIterAnterior)
                            *(
                                (gridCell->eIterAnterior) 
                                    +
                                (
                                    (gridCell->uIterAnterior)   
                                   *(gridCell->uIterAnterior)
                                        +
                                    (gridCell->vIterAnterior)
                                   *(gridCell->vIterAnterior)
                                )/2.0
                            )
                                 + 
                        peso*(gridCell->northRho)
                            *(gridCell->northV)
                            *(
                                (gridCell->northE) 
                                    +
                                (
                                    (gridCell->northU)   
                                   *(gridCell->northU)
                                        +
                                    (gridCell->northV)
                                   *(gridCell->northV)
                                )/2.0
                             )
                           ) / (1.0 + peso);

                if(expression == 18 /* p v */)
                    return ((gridCell->pIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->northP)*(gridCell->northV)
                           ) / (1.0 + peso);
                
            break;
                       /* ----------------------------------------------- */
            case 's':  /* ------------------- SUL ----------------------- */
                if(expression == 5 /* rho v */ )
                    return ((gridCell->rhoIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->southRho)*(gridCell->southV)
                           ) / (1.0 + peso);

                if(expression == 6 /* rho u v */ )
                    return ((gridCell->rhoIterAnterior)
                            *(gridCell->uIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->southRho)
                            *(gridCell->southU)*(gridCell->southV)
                           ) / (1.0 + peso);

                if(expression == 7 /* tau_xy */ )
                    return ( tauXY(gridCell,'0')
                                 + 
                             peso*tauXY(gridCell->south,'0')
                           ) / (1.0 + peso);

                if(expression == 3 /* p */)
                    return ((gridCell->pIterAnterior)         
                                 + 
                             peso*(gridCell->southP)
                           ) / (1.0 + peso);

                if(expression == 8 /* rho v^2 */ )
                    return ((gridCell->rhoIterAnterior)
                            *(gridCell->vIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->southRho)
                            *(gridCell->southV)*(gridCell->southV)
                           ) / (1.0 + peso);

                if(expression == 13 /* u tau_xy */)
                    return ( (gridCell->uIterAnterior)
                             *tauXY(gridCell,'0')
                                 + 
                             peso*(gridCell->southU)
                             *tauXY(gridCell->south,'0')
                           ) / (1.0 + peso);

                if(expression == 14 /* v tau_yy */)
                    return ( (gridCell->vIterAnterior)
                             *tauYY(gridCell,'0')
                                 + 
                             peso*(gridCell->southV)
                             *tauYY(gridCell->south,'0')
                           ) / (1.0 + peso);

                if(expression == 16 /* qY */ )
                    return ( qY(gridCell,'0')
                                 + 
                             peso*qY(gridCell->south,'0')
                           ) / (1.0 + peso);

                if(expression == 17 /* Et v */ )
                    return( (gridCell->rhoIterAnterior)
                            *(gridCell->vIterAnterior)
                            *(
                                (gridCell->eIterAnterior) 
                                    +
                                (
                                    (gridCell->uIterAnterior)   
                                   *(gridCell->uIterAnterior)
                                        +
                                    (gridCell->vIterAnterior)
                                   *(gridCell->vIterAnterior)
                                )/2.0
                            )
                                 + 
                        peso*(gridCell->southRho)
                            *(gridCell->southV)
                            *(
                                (gridCell->southE) 
                                    +
                                (
                                    (gridCell->southU)   
                                   *(gridCell->southU)
                                        +
                                    (gridCell->southV)
                                   *(gridCell->southV)
                                )/2.0
                             )
                           ) / (1.0 + peso);

                if(expression == 18 /* p v */)
                    return ((gridCell->pIterAnterior)*(gridCell->vIterAnterior)
                                 + 
                             peso*(gridCell->southP)*(gridCell->southV)
                           ) / (1.0 + peso);
                
            break;
            
            default: break;
        }
    
    }

  // Para vizinhos que pertencem 'a fronteira ---------------------------------
    if( ( neighborGridCell->type == 'w' ) || ( neighborGridCell->active == false ) )
    {   
        switch(direction)
        {             /* ----------------------------------------------- */
            case 'w': /* ------------------ OESTE ---------------------- */     
                if(expression == 1 /* rho u */)
                    return (gridCell->westRho)*(gridCell->westU);

                if(expression == 2 /*rho u^2*/)
                    return (gridCell->westRho)
                           *(gridCell->westU)*(gridCell->westU);

                if(expression == 3 /* p */)
                    return (gridCell->westP);
        
                if(expression == 4 /* tau_xx */)
                    return (tauXX(gridCell,'w'));

                if(expression == 6 /* rho u v */)
                    return (gridCell->westRho)
                           *(gridCell->westU)*(gridCell->westV);
                
                if(expression == 7 /* tau_xy */ )
                    return (tauXY(gridCell,'w'));

                if(expression == 9 /* Et u */ )
                    return (gridCell->westRho)
                            *(gridCell->westU)
                            *(
                                (gridCell->westE) 
                                    +
                                (
                                    (gridCell->westU)   
                                   *(gridCell->westU)
                                        +
                                    (gridCell->westV)
                                   *(gridCell->westV)
                                )/2.0
                             );

                if(expression == 10 /* p u */)
                    return (gridCell->westP)*(gridCell->westU);
    
                if(expression == 11 /* u tau_xx */)
                    return (gridCell->westU)
                           *tauXX(gridCell,'w');

                if(expression == 12 /* v tau_xy */)
                    return (gridCell->westV)
                           *tauXY(gridCell,'w');

                if(expression == 15 /* qX */)
                    return (qX(gridCell,'w'));
                           
            break;
                      /* ----------------------------------------------- */
            case 'e': /* ------------------ LESTE ---------------------- */
                if(expression == 1 /* rho u */)
                    return (gridCell->eastRho)*(gridCell->eastU);

                if(expression == 2 /*rho u^2*/)
                    return (gridCell->eastRho)
                           *(gridCell->eastU)*(gridCell->eastU);

                if(expression == 3 /* p */)
                    return (gridCell->eastP);

                if(expression == 4 /* tau_xx */)
                    return (tauXX(gridCell,'e'));

                if(expression == 6 /* rho u v */)
                    return (gridCell->eastRho)
                           *(gridCell->eastU)*(gridCell->eastV);

                if(expression == 7 /* tau_xy */ )
                    return (tauXY(gridCell,'e'));

                if(expression == 9 /* Et u */ )
                    return (gridCell->eastRho)
                            *(gridCell->eastU)
                            *(
                                (gridCell->eastE) 
                                    +
                                (
                                    (gridCell->eastU)   
                                   *(gridCell->eastU)
                                        +
                                    (gridCell->eastV)
                                   *(gridCell->eastV)
                                )/2.0
                             );

                if(expression == 10 /* p u */)
                    return (gridCell->eastP)*(gridCell->eastU);

                if(expression == 11 /* u tau_xx */)
                    return (gridCell->eastU)
                           *tauXX(gridCell,'e');

                if(expression == 12 /* v tau_xy */)
                    return (gridCell->eastV)
                           *tauXY(gridCell,'e');

                if(expression == 15 /* qX */)
                    return (qX(gridCell,'e')); 

            break;
                      /* ----------------------------------------------- */
            case 'n': /* ------------------ NORTE ---------------------- */
                if(expression == 5 /* rho v */)
                    return (gridCell->northRho)*(gridCell->northV);

                if(expression == 6 /* rho u v */)
                    return (gridCell->northRho)
                           *(gridCell->northU)*(gridCell->northV);

                if(expression == 3 /* p */)
                    return (gridCell->northP);

                if(expression == 7 /* tau_xy */ )
                    return (tauXY(gridCell,'n'));

                if(expression == 8 /*rho v^2*/)
                    return (gridCell->northRho)
                           *(gridCell->northV)*(gridCell->northV);

                if(expression == 13 /* u tau_xy */)
                    return (gridCell->northU)
                           *tauXY(gridCell,'n');

                if(expression == 14 /* v tau_yy */)
                    return (gridCell->northV)
                           *tauYY(gridCell,'n');

                if(expression == 16 /* qY */ )
                    return (qY(gridCell,'n'));

                if(expression == 17 /* Et v */ )
                    return (gridCell->northRho)
                            *(gridCell->northV)
                            *(
                                (gridCell->northE) 
                                    +
                                (
                                    (gridCell->northU)   
                                   *(gridCell->northU)
                                        +
                                    (gridCell->northV)
                                   *(gridCell->northV)
                                )/2.0
                             );

                if(expression == 18 /* p v */)
                    return (gridCell->northP)*(gridCell->northV);
                
            break;
                      /* ----------------------------------------------- */
            case 's': /* ------------------- SUL ----------------------- */
                if(expression == 5 /* rho v */)
                    return (gridCell->southRho)*(gridCell->southV);

                if(expression == 6 /* rho u v */)
                    return (gridCell->southRho)
                           *(gridCell->southU)*(gridCell->southV);

                if(expression == 3 /* p */)
                    return (gridCell->southP);
    
                if(expression == 7 /* tau_xy */ )
                    return (tauXY(gridCell,'s'));

                if(expression == 8 /*rho v^2*/)
                    return (gridCell->southRho)
                           *(gridCell->southV)*(gridCell->southV);

                if(expression == 13 /* u tau_xy */)
                    return (gridCell->southU)
                           *tauXY(gridCell,'s');

                if(expression == 14 /* v tau_yy */)
                    return (gridCell->southV)
                           *tauYY(gridCell,'s');

                if(expression == 16 /* qY */ )
                    return (qY(gridCell,'s'));

                if(expression == 17 /* Et v */ )
                    return (gridCell->southRho)
                            *(gridCell->southV)
                            *(
                                (gridCell->southE) 
                                    +
                                (
                                    (gridCell->southU)   
                                   *(gridCell->southU)
                                        +
                                    (gridCell->southV)
                                   *(gridCell->southV)
                                )/2.0
                             );

                if(expression == 18 /* p v */)
                    return (gridCell->southP)*(gridCell->southV);
         
            break;
            
            default: break;
        }     
    }
}
