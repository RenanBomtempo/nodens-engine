//derefinement.cpp
//Derefinement functions.

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;

#include <cmath>

/*______________________________________________________________________________
________________________________________________________________________________

                        FUNCTION derefineGrid()
                        
    Decides if the grid should be derefined by traversing the whole grid, 
	according to the parameter derefinementBound.
    
________________________________________________________________________________
______________________________________________________________________________*/
void derefineGrid( Grid *grid, double derefinementBound, bool drawGrid )
{
    Cell *gridCell,
    	 *auxiliarGridCell;

    int auxiliarNumber1, 
        auxiliarNumber2, 
        auxiliarNumber3, 
        auxiliarNumber4;
    bool hasBeenDerefined;
     
    // Percorrer todas as celulas
    gridCell = grid->firstGridCell;
    while( gridCell != NULL )
    {
        hasBeenDerefined = false;
        if( gridCell->level > REFINAMENTO_MIN )
        {
		 	// Buscar cachos que nao tem ficticios
            if( (gridCell->next != NULL) 
            &&  (gridCell->next->next != NULL) 
            &&  (gridCell->next->next->next != NULL) )
            {
			 	if( (gridCell->level == gridCell->next->level)
                &&  (gridCell->level == gridCell->next->next->level) 
                &&  (gridCell->level == gridCell->next->next->next->level) ) 
                {
                    //Checks if this cell together with the next three cells
                    //belong to the same bunch.
                    auxiliarNumber1 = (gridCell->bunchNumber)/10;
                    auxiliarNumber2 = (gridCell->next->bunchNumber)/10;
                    auxiliarNumber3 = (gridCell->next->next->bunchNumber)/10;
                    auxiliarNumber4 = (gridCell->next->next->next->bunchNumber)/10;  

                    if( (auxiliarNumber1 == auxiliarNumber2) 
                    &&  (auxiliarNumber1 == auxiliarNumber3) 
                    &&  (auxiliarNumber1 == auxiliarNumber4)   )
                    {
                        /* Notice that by the ordering conferred by the Hilbert 
						   curve, the program always enters the bunch to be 
						   derefined by its first member cell.
                           */
                        if( needsDerefinement( gridCell, derefinementBound ) )
                        {
                           // Ter referencia para proxima celula depois do cacho
                            auxiliarGridCell = gridCell->next->next->next->next;

                            // Desrefinar            
                            derefineBunch( gridCell, false );
                            grid->orderGridCells();
                            grid->numberOfCells -= 3;
    
                            // Continuar no loop
                            gridCell = auxiliarGridCell;
                            hasBeenDerefined = true;
                        }
                    }
                }             
            }
        }
        if( hasBeenDerefined == false )
            gridCell = gridCell->next;
        
    }
    grid->orderGridCells();

    for( gridCell = grid->firstGridCell;
        gridCell != NULL;
        gridCell = gridCell->next)
    {
        if( gridCell->active )//&& !gridCell->volumeFicticio)
        {
            gridCell->northFaceRho = interpolaVariavelFace( gridCell, ID_VAR_RHO, 'n' );
            gridCell->southFaceRho = interpolaVariavelFace( gridCell, ID_VAR_RHO, 's' );
            gridCell->eastFaceRho  = interpolaVariavelFace( gridCell, ID_VAR_RHO, 'e' );
            gridCell->westFaceRho  = interpolaVariavelFace( gridCell, ID_VAR_RHO, 'w' );
            gridCell->northFaceU   = interpolaVariavelFace( gridCell, ID_VAR_U, 'n' );
            gridCell->southFaceU   = interpolaVariavelFace( gridCell, ID_VAR_U, 's' );
            gridCell->eastFaceU    = interpolaVariavelFace( gridCell, ID_VAR_U, 'e' );
            gridCell->westFaceU    = interpolaVariavelFace( gridCell, ID_VAR_U, 'w' );
            gridCell->northFaceV   = interpolaVariavelFace( gridCell, ID_VAR_V, 'n' );
            gridCell->southFaceV   = interpolaVariavelFace( gridCell, ID_VAR_V, 's' );
            gridCell->eastFaceV    = interpolaVariavelFace( gridCell, ID_VAR_V, 'e' );
            gridCell->westFaceV    = interpolaVariavelFace( gridCell, ID_VAR_V, 'w' );
            gridCell->northFaceT   = interpolaVariavelFace( gridCell, ID_VAR_T, 'n' );
            gridCell->southFaceT   = interpolaVariavelFace( gridCell, ID_VAR_T, 's' );
            gridCell->eastFaceT    = interpolaVariavelFace( gridCell, ID_VAR_T, 'e' );
            gridCell->westFaceT    = interpolaVariavelFace( gridCell, ID_VAR_T, 'w' );
            gridCell->northFaceP   = interpolaVariavelFace( gridCell, ID_VAR_P, 'n' );
            gridCell->southFaceP   = interpolaVariavelFace( gridCell, ID_VAR_P, 's' );
            gridCell->eastFaceP    = interpolaVariavelFace( gridCell, ID_VAR_P, 'e' );
            gridCell->westFaceP    = interpolaVariavelFace( gridCell, ID_VAR_P, 'w' );
            gridCell->northFaceMi  = interpolaVariavelFace( gridCell, ID_VAR_MI, 'n' );
            gridCell->southFaceMi  = interpolaVariavelFace( gridCell, ID_VAR_MI, 's' );
            gridCell->eastFaceMi   = interpolaVariavelFace( gridCell, ID_VAR_MI, 'e' );
            gridCell->westFaceMi   = interpolaVariavelFace( gridCell, ID_VAR_MI, 'w' );
            gridCell->northFaceK   = interpolaVariavelFace( gridCell, ID_VAR_K, 'n' );
            gridCell->southFaceK   = interpolaVariavelFace( gridCell, ID_VAR_K, 's' );
            gridCell->eastFaceK    = interpolaVariavelFace( gridCell, ID_VAR_K, 'e' );
            gridCell->westFaceK    = interpolaVariavelFace( gridCell, ID_VAR_K, 'w' );
        }
    }

}//end derefineGrid() function


/*______________________________________________________________________________
________________________________________________________________________________
            					
								FUNCTION derefineBunch()
            					
    Derefines a bunch of four cells, tranforming it into a unique cell.
    Receives as an argument the first cell of the bunch.

________________________________________________________________________________
______________________________________________________________________________*/

void derefineBunch( Cell *cell, bool drawGrid )
{
    int bunchLevel;
    Cell *firstCell,
  		 *secondCell,
         *thirdCell,
         *fourthCell,
         *newCell;
    Cell *cellBeforeBunch, 
         *cellAfterBunch;
        
    double NOVOrhoAtual,
           NOVOrhoAnterior,
           NOVOrhoRetrasado,
           NOVOrhoIterAnterior,
           NOVOuAtual,
           NOVOuAnterior,
           NOVOuRetrasado,
           NOVOuIterAnterior,
           NOVOvAtual,
           NOVOvAnterior,
           NOVOvRetrasado,
           NOVOvIterAnterior,
           NOVOTAtual,
           NOVOTAnterior,
           NOVOTRetrasado,
           NOVOTIterAnterior,
           NOVOPhiAtual,
           NOVOPhiAnterior,
           NOVOPhiRetrasado,
           NOVOpAtual,
           NOVOpAnterior,
           NOVOpRetrasado,
           NOVOpIterAnterior,
           NOVOpLinha,
           NOVOmiAtual,
           NOVOmiAnterior,
           NOVOmiRetrasado,
           NOVOmiIterAnterior,
           NOVOkAtual,
           NOVOkAnterior,
           NOVOd_n,
           NOVOd_s,
           NOVOd_e,
           NOVOd_w;
          
    bunchLevel = cell->level;
         
    firstCell = cell;
    secondCell = cell->next;
    thirdCell = cell->next->next;
    fourthCell = cell->next->next->next;
    
    cellBeforeBunch = firstCell->previous;
    cellAfterBunch = fourthCell->next;
        
    //Determines which is the least hilbertShapeNumber.
    int leastHilbertShapeNumber;             
    leastHilbertShapeNumber = firstCell->hilbertShapeNumber;
    if( secondCell->hilbertShapeNumber < leastHilbertShapeNumber )
        leastHilbertShapeNumber = secondCell->hilbertShapeNumber;
    if( thirdCell->hilbertShapeNumber < leastHilbertShapeNumber )
        leastHilbertShapeNumber = thirdCell->hilbertShapeNumber;
    if( fourthCell->hilbertShapeNumber < leastHilbertShapeNumber )
        leastHilbertShapeNumber = fourthCell->hilbertShapeNumber;
    
    //New variable values of derefined cell. Arithmetic mean.
    NOVOrhoAtual = 
        (   firstCell->rhoAtual + secondCell->rhoAtual 
          + thirdCell->rhoAtual + fourthCell->rhoAtual 
        ) /4;
    NOVOrhoAnterior = 
        (   firstCell->rhoAnterior + secondCell->rhoAnterior 
          + thirdCell->rhoAnterior + fourthCell->rhoAnterior 
        ) /4;

    NOVOrhoRetrasado = 
        (   firstCell->rhoRetrasado + secondCell->rhoRetrasado 
          + thirdCell->rhoRetrasado + fourthCell->rhoRetrasado 
        ) /4;
    NOVOrhoIterAnterior = 
        (   firstCell->rhoIterAnterior + secondCell->rhoIterAnterior 
          + thirdCell->rhoIterAnterior + fourthCell->rhoIterAnterior 
        ) /4;
    NOVOuAtual = 
        (   firstCell->uAtual + secondCell->uAtual 
          + thirdCell->uAtual + fourthCell->uAtual 
        ) /4;
    NOVOuAnterior = 
        (   firstCell->uAnterior + secondCell->uAnterior 
          + thirdCell->uAnterior + fourthCell->uAnterior 
        ) /4;
    NOVOuRetrasado = 
        (   firstCell->uRetrasado + secondCell->uRetrasado 
          + thirdCell->uRetrasado + fourthCell->uRetrasado 
        ) /4;
    NOVOuIterAnterior = 
        (   firstCell->uIterAnterior + secondCell->uIterAnterior 
          + thirdCell->uIterAnterior + fourthCell->uIterAnterior 
        ) /4;
    NOVOvAtual = 
        (   firstCell->vAtual + secondCell->vAtual 
          + thirdCell->vAtual + fourthCell->vAtual 
        ) /4;
    NOVOvAnterior = 
        (   firstCell->vAnterior + secondCell->vAnterior 
          + thirdCell->vAnterior + fourthCell->vAnterior 
        ) /4;
    NOVOvRetrasado = 
        (   firstCell->vRetrasado + secondCell->vRetrasado 
          + thirdCell->vRetrasado + fourthCell->vRetrasado 
        ) /4;
    NOVOvIterAnterior = 
        (   firstCell->vIterAnterior + secondCell->vIterAnterior 
          + thirdCell->vIterAnterior + fourthCell->vIterAnterior 
        ) /4;
    NOVOTAtual = 
        (   firstCell->TAtual + secondCell->TAtual 
          + thirdCell->TAtual + fourthCell->TAtual 
        ) /4;
    NOVOTAnterior = 
        (   firstCell->TAnterior + secondCell->TAnterior 
          + thirdCell->TAnterior + fourthCell->TAnterior 
        ) /4;
    NOVOTRetrasado = 
        (   firstCell->TRetrasado + secondCell->TRetrasado 
          + thirdCell->TRetrasado + fourthCell->TRetrasado 
        ) /4;
    NOVOTIterAnterior = 
        (   firstCell->TIterAnterior + secondCell->TIterAnterior 
          + thirdCell->TIterAnterior + fourthCell->TIterAnterior 
        ) /4;
    NOVOPhiAtual = 
        (   firstCell->PhiAtual + secondCell->PhiAtual 
          + thirdCell->PhiAtual + fourthCell->PhiAtual 
        ) /4;
    NOVOPhiRetrasado = 
        (   firstCell->PhiRetrasado + secondCell->PhiRetrasado 
          + thirdCell->PhiRetrasado + fourthCell->PhiRetrasado 
        ) /4;
    NOVOPhiAnterior = 
        (   firstCell->PhiAnterior + secondCell->PhiAnterior 
          + thirdCell->PhiAnterior + fourthCell->PhiAnterior
        ) /4;
    NOVOpAtual = 
        (   firstCell->pAtual + secondCell->pAtual 
          + thirdCell->pAtual + fourthCell->pAtual 
        ) /4;
    NOVOpAnterior = 
        (   firstCell->pAnterior + secondCell->pAnterior 
          + thirdCell->pAnterior + fourthCell->pAnterior 
        ) /4;
    NOVOpRetrasado = 
        (   firstCell->pRetrasado + secondCell->pRetrasado 
          + thirdCell->pRetrasado + fourthCell->pRetrasado 
        ) /4;
    NOVOpIterAnterior = 
        (   firstCell->pIterAnterior + secondCell->pIterAnterior 
          + thirdCell->pIterAnterior + fourthCell->pIterAnterior 
        ) /4;
    NOVOpLinha = 
        (   firstCell->pLinha + secondCell->pLinha 
          + thirdCell->pLinha + fourthCell->pLinha 
        ) /4;
    NOVOmiAtual = 
        (   firstCell->miAtual + secondCell->miAtual 
          + thirdCell->miAtual + fourthCell->miAtual 
        ) /4;
    NOVOmiAnterior = 
        (   firstCell->miAnterior + secondCell->miAnterior 
          + thirdCell->miAnterior + fourthCell->miAnterior 
        ) /4;
    NOVOmiRetrasado = 
        (   firstCell->miRetrasado + secondCell->miRetrasado 
          + thirdCell->miRetrasado + fourthCell->miRetrasado 
        ) /4;
    NOVOmiIterAnterior = 
        (   firstCell->miIterAnterior + secondCell->miIterAnterior 
          + thirdCell->miIterAnterior + fourthCell->miIterAnterior 
        ) /4;
    NOVOkAtual = 
        (   firstCell->pAtual + secondCell->pAtual 
          + thirdCell->pAtual + fourthCell->pAtual 
        ) /4;
    NOVOkAnterior = 
        (   firstCell->pAnterior + secondCell->pAnterior 
          + thirdCell->pAnterior + fourthCell->pAnterior 
        ) /4;
    NOVOd_n = 
        (   firstCell->d_n + secondCell->d_n 
          + thirdCell->d_n + fourthCell->d_n 
        ) /4;
    NOVOd_s = 
        (   firstCell->d_s + secondCell->d_s 
          + thirdCell->d_s + fourthCell->d_s
        ) /4;
    NOVOd_e = 
        (   firstCell->d_e + secondCell->d_e 
          + thirdCell->d_e + fourthCell->d_e
        ) /4;
    NOVOd_w = 
        (   firstCell->d_w + secondCell->d_w 
          + thirdCell->d_w + fourthCell->d_w
        ) /4;

    /*__________________________________________________________________________
               	NORTHEAST NODE OF THE BUNCH BECOMES DEREFINED NODE
    __________________________________________________________________________*/
    //Determines which is the northeast cell of the bunch.    
    double coordinateSum1,
           coordinateSum2,
           coordinateSum3,
           coordinateSum4;
    coordinateSum1 = firstCell->centerX + firstCell->centerY;    
    coordinateSum2 = secondCell->centerX + secondCell->centerY;    
    coordinateSum3 = thirdCell->centerX + thirdCell->centerY;
    coordinateSum4 = fourthCell->centerX + fourthCell->centerY;
    
    double maximum;
    newCell = firstCell;
    maximum = coordinateSum1;
    if( coordinateSum2 > maximum )
    {
        maximum = coordinateSum2;
        newCell = secondCell;
    }
    if( coordinateSum3 > maximum )
    {
        maximum = coordinateSum3;
        newCell = thirdCell;
    }
    if( coordinateSum4 > maximum )
    {
        maximum = coordinateSum4;
        newCell = fourthCell;
    }
    
    /*__________________________________________________________________________
               	REDEFINING POINTERS OF NORTHEAST NODE AND NEIGHBORS
    __________________________________________________________________________*/
    newCell->previous = cellBeforeBunch;
    newCell->next = cellAfterBunch;
    if( newCell->previous != 0 )
        newCell->previous->next = newCell;
    if( newCell->next != 0 )
        newCell->next->previous = newCell;
    
    //Resetting of new geometric variables.
    newCell->centerX = ( newCell->centerX + newCell->west->centerX ) / 2.0;
    newCell->centerY = ( newCell->centerY + newCell->south->centerY ) / 2.0;             
    //Erases inner boundaries of bunch cells.
    if( drawGrid )
    {
        setColor(7);
   	    //Erases "+" in the center of the bunch.
   	    double halfSideBunchLength = newCell->faceLength;
   	    drawLine( newCell->centerX, newCell->centerY + halfSideBunchLength,0,
                  newCell->centerX, newCell->centerY - halfSideBunchLength,0 );
   	    drawLine( newCell->centerX + halfSideBunchLength, newCell->centerY,0,
                  newCell->centerX - halfSideBunchLength, newCell->centerY,0 );
    }
    newCell->faceLength = 2 * newCell->faceLength;
    newCell->halfFaceLength = 2 * newCell->halfFaceLength;
    
    newCell->level = bunchLevel - 1;
    newCell->bunchNumber = newCell->bunchNumber/10; 
    newCell->hilbertShapeNumber = leastHilbertShapeNumber;
    newCell->rhoAtual = NOVOrhoAtual;
    newCell->rhoAnterior = NOVOrhoAnterior;
    newCell->rhoRetrasado = NOVOrhoRetrasado;
    newCell->rhoIterAnterior = NOVOrhoIterAnterior;
    newCell->uAtual = NOVOuAtual;
    newCell->uAnterior = NOVOuAnterior;
    newCell->uRetrasado = NOVOuRetrasado;
    newCell->uIterAnterior = NOVOuIterAnterior;
    newCell->vAtual = NOVOvAtual;
    newCell->vAnterior = NOVOvAnterior;
    newCell->vRetrasado = NOVOvRetrasado;
    newCell->vIterAnterior = NOVOvIterAnterior;
    newCell->TAtual = NOVOTAtual;
    newCell->TAnterior = NOVOTAnterior;
    newCell->TRetrasado = NOVOTRetrasado;
    newCell->TIterAnterior = NOVOTIterAnterior;
    newCell->PhiAtual = NOVOPhiAtual;
    newCell->PhiAnterior = NOVOPhiAnterior;
    newCell->PhiRetrasado = NOVOPhiRetrasado;
    newCell->pAtual = NOVOpAtual;
    newCell->pAnterior = NOVOpAnterior;
    newCell->pRetrasado = NOVOpRetrasado;
    newCell->pIterAnterior = NOVOpIterAnterior;
    newCell->pLinha = NOVOpLinha;
    newCell->miAtual = NOVOmiAtual;
    newCell->miAnterior = NOVOmiAnterior;
    newCell->miRetrasado = NOVOmiRetrasado;
    newCell->miIterAnterior = NOVOmiIterAnterior;
    newCell->kAtual = NOVOkAtual;
    newCell->kAnterior = NOVOkAnterior;

    newCell->northFaceRho = interpolaVariavelFace( newCell, ID_VAR_RHO, 'n' );
    newCell->southFaceRho = interpolaVariavelFace( newCell, ID_VAR_RHO, 's' );
    newCell->eastFaceRho  = interpolaVariavelFace( newCell, ID_VAR_RHO, 'e' );
    newCell->westFaceRho  = interpolaVariavelFace( newCell, ID_VAR_RHO, 'w' );
    newCell->northFaceU   = interpolaVariavelFace( newCell, ID_VAR_U, 'n' );
    newCell->southFaceU   = interpolaVariavelFace( newCell, ID_VAR_U, 's' );
    newCell->eastFaceU    = interpolaVariavelFace( newCell, ID_VAR_U, 'e' );
    newCell->westFaceU    = interpolaVariavelFace( newCell, ID_VAR_U, 'w' );
    newCell->northFaceV   = interpolaVariavelFace( newCell, ID_VAR_V, 'n' );
    newCell->southFaceV   = interpolaVariavelFace( newCell, ID_VAR_V, 's' );
    newCell->eastFaceV    = interpolaVariavelFace( newCell, ID_VAR_V, 'e' );
    newCell->westFaceV    = interpolaVariavelFace( newCell, ID_VAR_V, 'w' );
    newCell->northFaceT   = interpolaVariavelFace( newCell, ID_VAR_T, 'n' );
    newCell->southFaceT   = interpolaVariavelFace( newCell, ID_VAR_T, 's' );
    newCell->eastFaceT    = interpolaVariavelFace( newCell, ID_VAR_T, 'e' );
    newCell->westFaceT    = interpolaVariavelFace( newCell, ID_VAR_T, 'w' );
    newCell->northFaceP   = interpolaVariavelFace( newCell, ID_VAR_P, 'n' );
    newCell->southFaceP   = interpolaVariavelFace( newCell, ID_VAR_P, 's' );
    newCell->eastFaceP    = interpolaVariavelFace( newCell, ID_VAR_P, 'e' );
    newCell->westFaceP    = interpolaVariavelFace( newCell, ID_VAR_P, 'w' );
    newCell->northFaceMi  = interpolaVariavelFace( newCell, ID_VAR_MI, 'n' );
    newCell->southFaceMi  = interpolaVariavelFace( newCell, ID_VAR_MI, 's' );
    newCell->eastFaceMi   = interpolaVariavelFace( newCell, ID_VAR_MI, 'e' );
    newCell->westFaceMi   = interpolaVariavelFace( newCell, ID_VAR_MI, 'w' );
    newCell->northFaceK   = interpolaVariavelFace( newCell, ID_VAR_K, 'n' );
    newCell->southFaceK   = interpolaVariavelFace( newCell, ID_VAR_K, 's' );
    newCell->eastFaceK    = interpolaVariavelFace( newCell, ID_VAR_K, 'e' );
    newCell->westFaceK    = interpolaVariavelFace( newCell, ID_VAR_K, 'w' );
    
    /*__________________________________________________________________________
               	             CREATION OF 4 NEW TRANSITION NODES
    __________________________________________________________________________*/
    Cell *northTransitionNode,
  		 *southTransitionNode,
         *eastTransitionNode,
         *westTransitionNode;
         
    northTransitionNode = new Cell;
    southTransitionNode = new Cell;
    eastTransitionNode = new Cell;
    westTransitionNode = new Cell;
    
    northTransitionNode->type = 'w';
    southTransitionNode->type = 'w';
    eastTransitionNode->type = 'w';
    westTransitionNode->type = 'w';
    
    northTransitionNode->level = bunchLevel;
    southTransitionNode->level = bunchLevel;
    eastTransitionNode->level = bunchLevel;
    westTransitionNode->level = bunchLevel;
    
    northTransitionNode->singleConnector = newCell;
    southTransitionNode->singleConnector = newCell;
    eastTransitionNode->singleConnector = newCell;
    westTransitionNode->singleConnector = newCell;
    
    northTransitionNode->doubleConnector1 = newCell->north;
    northTransitionNode->doubleConnector2 = newCell->west->north;
    southTransitionNode->doubleConnector1 = newCell->south->south;
    southTransitionNode->doubleConnector2 = newCell->south->west->south;
    eastTransitionNode->doubleConnector1 = newCell->east;
    eastTransitionNode->doubleConnector2 = newCell->south->east;
    westTransitionNode->doubleConnector1 = newCell->west->west;
    westTransitionNode->doubleConnector2 = newCell->west->south->west;
    
    /*__________________________________________________________________________
                       ELIMINATION OF THE THREE UNNEEDED BUNCH CELLS.
    __________________________________________________________________________*/
    delete newCell->west;
    delete newCell->south->west;
    delete newCell->south;        
                    
    /*__________________________________________________________________________
                       LINKING OF DEREFINED CELL AND NEW TRANSITION NODES
    __________________________________________________________________________*/
    newCell->north = northTransitionNode;
    newCell->south = southTransitionNode;
    newCell->east = eastTransitionNode;
    newCell->west = westTransitionNode;                      
    
    /*__________________________________________________________________________
                   SIMPLIFICATION AND/OR CONNECTION OF TRANSITION NODES
                       
    Simplifies the graph by eliminating adjacent transition nodes
    of same level connected through their double connectors.
    If a transition node is not eliminated, because there is no adjacent transition
    node of same level, then simply connects the outside to it.      
    __________________________________________________________________________*/
    bool isConnected = false;
    //__________________________________________________________________________
    //North transition node.
    if( ( northTransitionNode->doubleConnector1->type == 'w' ) 
    &&  ( northTransitionNode->doubleConnector2->type == 'w' ) )
    {
        if( ( northTransitionNode->doubleConnector1->level == bunchLevel )
        &&  ( northTransitionNode->doubleConnector2->level == bunchLevel ) )
        {
            newCell->north = northTransitionNode->doubleConnector2->singleConnector;
            if( northTransitionNode->doubleConnector2->singleConnector->type == 'b')
                northTransitionNode->doubleConnector2->singleConnector->south = newCell;
            if( northTransitionNode->doubleConnector2->singleConnector->type == 'w')
            {
                if(northTransitionNode->doubleConnector2->singleConnector->doubleConnector1
                   == northTransitionNode->doubleConnector2)
                   northTransitionNode->doubleConnector2->singleConnector->doubleConnector1  
                   = newCell;
                if(northTransitionNode->doubleConnector2->singleConnector->doubleConnector2
                   == northTransitionNode->doubleConnector2)
                   northTransitionNode->doubleConnector2->singleConnector->doubleConnector2  
                   = newCell;
            }

            delete northTransitionNode->doubleConnector2;
            delete northTransitionNode;
            
            isConnected = true;
        }
    } 
    //Connects outside to north transition node, if this was not deleted.
    if( isConnected == false )
    {
        if( ( northTransitionNode->doubleConnector1->type == 'w' )
        &&  ( northTransitionNode->doubleConnector1->level > bunchLevel ) )
              northTransitionNode->doubleConnector1->singleConnector 
              = northTransitionNode;
        if( ( northTransitionNode->doubleConnector2->type == 'w' )
        &&  ( northTransitionNode->doubleConnector2->level > bunchLevel ) )
              northTransitionNode->doubleConnector2->singleConnector 
              = northTransitionNode;
        
        if( northTransitionNode->doubleConnector1->type == 'b' )
            northTransitionNode->doubleConnector1->south
            = northTransitionNode;      
        if( northTransitionNode->doubleConnector2->type == 'b' )
            northTransitionNode->doubleConnector2->south
            = northTransitionNode;        
    }
    
    //__________________________________________________________________________
    //South transition node.
    isConnected = false;
    if( ( southTransitionNode->doubleConnector1->type == 'w' ) 
     && ( southTransitionNode->doubleConnector2->type == 'w' ) )
    {
        if( ( southTransitionNode->doubleConnector1->level == bunchLevel )
        &&  ( southTransitionNode->doubleConnector2->level == bunchLevel ) )
        {
            newCell->south = southTransitionNode->doubleConnector2->singleConnector;
            if( southTransitionNode->doubleConnector2->singleConnector->type == 'b')
                southTransitionNode->doubleConnector2->singleConnector->north = newCell;
            if( southTransitionNode->doubleConnector2->singleConnector->type == 'w')
            {
                if(southTransitionNode->doubleConnector2->singleConnector->doubleConnector1
                   == southTransitionNode->doubleConnector2)
                   southTransitionNode->doubleConnector2->singleConnector->doubleConnector1  
                   = newCell;
                if(southTransitionNode->doubleConnector2->singleConnector->doubleConnector2
                   == southTransitionNode->doubleConnector2)
                   southTransitionNode->doubleConnector2->singleConnector->doubleConnector2  
                   = newCell;
            }    

            delete southTransitionNode->doubleConnector2;
            delete southTransitionNode;
            
            isConnected = true;
        }
    }
    //Connects outside to south transition node, if this was not deleted.
    if( isConnected == false )
    {
        if( ( southTransitionNode->doubleConnector1->type == 'w' )
        &&  ( southTransitionNode->doubleConnector1->level > bunchLevel ) )
              southTransitionNode->doubleConnector1->singleConnector 
              = southTransitionNode;
        if( ( southTransitionNode->doubleConnector2->type == 'w' )
        &&  ( southTransitionNode->doubleConnector2->level > bunchLevel ) )
              southTransitionNode->doubleConnector2->singleConnector 
              = southTransitionNode;
        
        if( southTransitionNode->doubleConnector1->type == 'b' )
            southTransitionNode->doubleConnector1->north
            = southTransitionNode;      
        if( southTransitionNode->doubleConnector2->type == 'b' )
            southTransitionNode->doubleConnector2->north
            = southTransitionNode;        
    }
    
    //__________________________________________________________________________
    //East transition node.
    isConnected = false;    
    if( ( eastTransitionNode->doubleConnector1->type == 'w' ) 
     && ( eastTransitionNode->doubleConnector2->type == 'w' ) )
    {
        if( ( eastTransitionNode->doubleConnector1->level == bunchLevel )
        &&  ( eastTransitionNode->doubleConnector2->level == bunchLevel ) )
        {
            newCell->east = eastTransitionNode->doubleConnector2->singleConnector;
            if( eastTransitionNode->doubleConnector2->singleConnector->type == 'b')
                eastTransitionNode->doubleConnector2->singleConnector->west = newCell;
            if( eastTransitionNode->doubleConnector2->singleConnector->type == 'w')
            {
                if(eastTransitionNode->doubleConnector2->singleConnector->doubleConnector1
                   == eastTransitionNode->doubleConnector2)
                   eastTransitionNode->doubleConnector2->singleConnector->doubleConnector1  
                   = newCell;
                if(eastTransitionNode->doubleConnector2->singleConnector->doubleConnector2
                   == eastTransitionNode->doubleConnector2)
                   eastTransitionNode->doubleConnector2->singleConnector->doubleConnector2  
                   = newCell;
            }

            delete eastTransitionNode->doubleConnector2;
            delete eastTransitionNode;
            
            isConnected = true;
        }
    }
    //Connects outside to east transition node, if this was not deleted.
    if( isConnected == false )
    {
        if( ( eastTransitionNode->doubleConnector1->type == 'w' )
        &&  ( eastTransitionNode->doubleConnector1->level > bunchLevel ) )
              eastTransitionNode->doubleConnector1->singleConnector 
              = eastTransitionNode;
        if( ( eastTransitionNode->doubleConnector2->type == 'w' )
        &&  ( eastTransitionNode->doubleConnector2->level > bunchLevel ) )
              eastTransitionNode->doubleConnector2->singleConnector 
              = eastTransitionNode;
        
        if( eastTransitionNode->doubleConnector1->type == 'b' )
            eastTransitionNode->doubleConnector1->west
            = eastTransitionNode;      
        if( eastTransitionNode->doubleConnector2->type == 'b' )
            eastTransitionNode->doubleConnector2->west
            = eastTransitionNode;        
    }
    
    //__________________________________________________________________________
    //West transition node.
    isConnected = false;    
    if( ( westTransitionNode->doubleConnector1->type == 'w' ) 
     && ( westTransitionNode->doubleConnector2->type == 'w' ) )
    {
        if( ( westTransitionNode->doubleConnector1->level == bunchLevel )
        &&  ( westTransitionNode->doubleConnector2->level == bunchLevel ) )
        {
            newCell->west = westTransitionNode->doubleConnector2->singleConnector;
            if( westTransitionNode->doubleConnector2->singleConnector->type == 'b')
                westTransitionNode->doubleConnector2->singleConnector->east = newCell;
            if( westTransitionNode->doubleConnector2->singleConnector->type == 'w')
            {
                if(westTransitionNode->doubleConnector2->singleConnector->doubleConnector1
                   == westTransitionNode->doubleConnector2)
                   westTransitionNode->doubleConnector2->singleConnector->doubleConnector1  
                   = newCell;
                if(westTransitionNode->doubleConnector2->singleConnector->doubleConnector2
                   == westTransitionNode->doubleConnector2)
                   westTransitionNode->doubleConnector2->singleConnector->doubleConnector2  
                   = newCell;
            }

            delete westTransitionNode->doubleConnector2;
            delete westTransitionNode;
            
            isConnected = true;
        }
    }
    //Connects outside to west transition node, if this was not deleted.
    if( isConnected == false )
    {
        if( ( westTransitionNode->doubleConnector1->type == 'w' )
        &&  ( westTransitionNode->doubleConnector1->level > bunchLevel ) )
              westTransitionNode->doubleConnector1->singleConnector 
              = westTransitionNode;
        if( ( westTransitionNode->doubleConnector2->type == 'w' )
        &&  ( westTransitionNode->doubleConnector2->level > bunchLevel ) )
              westTransitionNode->doubleConnector2->singleConnector 
              = westTransitionNode;
        
        if( westTransitionNode->doubleConnector1->type == 'b' )
            westTransitionNode->doubleConnector1->east
            = westTransitionNode;     
        if( westTransitionNode->doubleConnector2->type == 'b' )
            westTransitionNode->doubleConnector2->east
            = westTransitionNode; 
    } 
}//end derefineBunch() function


/*______________________________________________________________________________
________________________________________________________________________________

                        FUNCTION needsDerefinement()
                        
    Decides if the bunch should be derefined.
    A bunch will not be derefined if the highest of all directional fluxes 
	coming into its four directions is greater than derefinementBound.
    
________________________________________________________________________________
______________________________________________________________________________*/
bool needsDerefinement( Cell *gridCell, double derefinementBound )
{
    Cell *firstCell,
  		 *secondCell,
         *thirdCell,
         *fourthCell;
    double maximumFaceDifferenceP = 0.0,
           maximumFaceDifferenceT = 0.0,
           maximumFaceDifferenceMach = 0.0;
    double maximum1_p, maximum1_T, maximum1_mach,
           maximum2_p, maximum2_T, maximum2_mach,
           maximum3_p, maximum3_T, maximum3_mach,
           maximum4_p, maximum4_T, maximum4_mach,
           highestMaximum_p, highestMaximum_T, highestMaximum_mach;
    bool derefinementCondition = false;
         
    firstCell = gridCell;
    secondCell = gridCell->next,
    thirdCell = gridCell->next->next,
    fourthCell = gridCell->next->next->next;

    if(firstCell->volumeFicticio 
    || secondCell->volumeFicticio
    || thirdCell->volumeFicticio
    || fourthCell->volumeFicticio )
        return false;
    
    //============================================================================================
    // maximum1 e' o maximo de p entre as faces do firstCell
    // Assume que o maior e' o do leste
    if( firstCell->pAtual != 0.0 )
        maximumFaceDifferenceP = fabs(
                        (firstCell->pAtual - firstCell->eastFaceP) / firstCell->pAtual
                    );
    else
        maximumFaceDifferenceP = fabs( firstCell->pAtual - firstCell->eastFaceP );

    if( firstCell->TAtual != 0.0 )
        maximumFaceDifferenceT = fabs(
                        (firstCell->TAtual - firstCell->eastFaceT) / firstCell->TAtual
                    );
    else
        maximumFaceDifferenceT = fabs( firstCell->TAtual - firstCell->eastFaceT );

    if( sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) != 0.0 )
        maximumFaceDifferenceMach = fabs(
        (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) -     // machAtual
        sqrt(AO_QUADRADO(firstCell->eastFaceU) + AO_QUADRADO(firstCell->eastFaceV)))    // eastFaceMach
        / (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)))
                    );
    else
        maximumFaceDifferenceMach = fabs(
        sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) -      // machAtual
        sqrt(AO_QUADRADO(firstCell->eastFaceU) + AO_QUADRADO(firstCell->eastFaceV)));   // eastFaceMach

    // Testa se a face norte e' maior
    if( firstCell->pAtual != 0.0 )
    {
        if( fabs( (firstCell->pAtual - firstCell->northFaceP) / firstCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (firstCell->pAtual - firstCell->northFaceP) / firstCell->pAtual );
    }
    else
    {
        if( fabs( (firstCell->pAtual - firstCell->northFaceP) ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( firstCell->pAtual - firstCell->northFaceP );
    }

    if( firstCell->TAtual != 0.0 )
    {
        if ( fabs( (firstCell->TAtual - firstCell->northFaceT) / firstCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (firstCell->TAtual - firstCell->northFaceT) / firstCell->TAtual );
    }
    else
    {
        if ( fabs( firstCell->TAtual - firstCell->northFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( firstCell->TAtual - firstCell->northFaceT );
    }


    if( sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) -
                    sqrt(AO_QUADRADO(firstCell->northFaceU) + AO_QUADRADO(firstCell->northFaceV) ) )
                    / sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(firstCell->northFaceU) + AO_QUADRADO(firstCell->northFaceV)))
                        // northFaceMach
                / (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) -
                    sqrt(AO_QUADRADO(firstCell->northFaceU) + AO_QUADRADO(firstCell->northFaceV)) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(firstCell->northFaceU) + AO_QUADRADO(firstCell->northFaceV)));
                        // northFaceMach
    }

    // Testa se a face oeste e' maior
    if( firstCell->pAtual != 0.0 )
    {
        if ( fabs( (firstCell->pAtual - firstCell->westFaceP) / firstCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (firstCell->pAtual - firstCell->westFaceP) / firstCell->pAtual );
    }
    else
    {
        if ( fabs( firstCell->pAtual - firstCell->westFaceP ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( firstCell->pAtual - firstCell->westFaceP );
    }

    if( firstCell->TAtual != 0.0 )
    {
        if ( fabs( (firstCell->TAtual - firstCell->westFaceT) / firstCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (firstCell->TAtual - firstCell->westFaceT) / firstCell->TAtual );
    }
    else
    {
        if ( fabs( firstCell->TAtual - firstCell->westFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( firstCell->TAtual - firstCell->westFaceT );
    }

    if( sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) -
                    sqrt(AO_QUADRADO(firstCell->westFaceU) + AO_QUADRADO(firstCell->westFaceV) ) )
                    / sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(firstCell->westFaceU) + AO_QUADRADO(firstCell->westFaceV)))
                        // westFaceMach
                / (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) -
                    sqrt(AO_QUADRADO(firstCell->westFaceU) + AO_QUADRADO(firstCell->westFaceV) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(firstCell->westFaceU) + AO_QUADRADO(firstCell->westFaceV)));
                        // westFaceMach
    }

    // Testa se a face sul e' maior
    if( firstCell->pAtual != 0.0 )
    {
        if ( fabs( (firstCell->pAtual - firstCell->southFaceP) / firstCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (firstCell->pAtual - firstCell->southFaceP) / firstCell->pAtual );
    }
    else
    {
        if ( fabs( firstCell->pAtual - firstCell->southFaceP ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( firstCell->pAtual - firstCell->southFaceP );
    }

    if( firstCell->TAtual != 0.0 )
    {
        if ( fabs( (firstCell->TAtual - firstCell->southFaceT) / firstCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (firstCell->TAtual - firstCell->southFaceT) / firstCell->TAtual );
    }
    else
    {
        if ( fabs( firstCell->TAtual - firstCell->southFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( firstCell->TAtual - firstCell->southFaceT );
    }

    if( sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) -
                    sqrt(AO_QUADRADO(firstCell->southFaceU) + AO_QUADRADO(firstCell->southFaceV) ) )
                    / sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(firstCell->southFaceU) + AO_QUADRADO(firstCell->southFaceV)))
                        // westFaceMach
                / (sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual) ) -
                    sqrt(AO_QUADRADO(firstCell->southFaceU) + AO_QUADRADO(firstCell->southFaceV) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(firstCell->uAtual) + AO_QUADRADO(firstCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(firstCell->southFaceU) + AO_QUADRADO(firstCell->southFaceV)));
                        // westFaceMach
    }

    maximum1_p = maximumFaceDifferenceP;
    maximum1_T = maximumFaceDifferenceT;
    maximum1_mach = maximumFaceDifferenceMach;

    //============================================================================================
    // maximum2 e' o maximo de p entre as faces do secondCell
    // Assume que o maior e' o do leste
    if( secondCell->pAtual != 0.0 )
        maximumFaceDifferenceP = fabs(
                        (secondCell->pAtual - secondCell->eastFaceP) / secondCell->pAtual
                    );
    else
        maximumFaceDifferenceP = fabs( secondCell->pAtual - secondCell->eastFaceP );

    if( secondCell->TAtual != 0.0 )
        maximumFaceDifferenceT = fabs(
                        (secondCell->TAtual - secondCell->eastFaceT) / secondCell->TAtual
                    );
    else
        maximumFaceDifferenceT = fabs( secondCell->TAtual - secondCell->eastFaceT );

    if( sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) != 0.0 )
        maximumFaceDifferenceMach = fabs(
        (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) -     // machAtual
        sqrt(AO_QUADRADO(secondCell->eastFaceU) + AO_QUADRADO(secondCell->eastFaceV)))    // eastFaceMach
        / (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)))
                    );
    else
        maximumFaceDifferenceMach = fabs(
        sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) -      // machAtual
        sqrt(AO_QUADRADO(secondCell->eastFaceU) + AO_QUADRADO(secondCell->eastFaceV)));   // eastFaceMach

    // Testa se a face norte e' maior
    if( secondCell->pAtual != 0.0 )
    {
        if( fabs( (secondCell->pAtual - secondCell->northFaceP) / secondCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (secondCell->pAtual - secondCell->northFaceP) / secondCell->pAtual );
    }
    else
    {
        if( fabs( (secondCell->pAtual - secondCell->northFaceP) ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( secondCell->pAtual - secondCell->northFaceP );
    }

    if( secondCell->TAtual != 0.0 )
    {
        if ( fabs( (secondCell->TAtual - secondCell->northFaceT) / secondCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (secondCell->TAtual - secondCell->northFaceT) / secondCell->TAtual );
    }
    else
    {
        if ( fabs( secondCell->TAtual - secondCell->northFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( secondCell->TAtual - secondCell->northFaceT );
    }


    if( sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) -
                    sqrt(AO_QUADRADO(secondCell->northFaceU) + AO_QUADRADO(secondCell->northFaceV) ) )
                    / sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(secondCell->northFaceU) + AO_QUADRADO(secondCell->northFaceV)))
                        // northFaceMach
                / (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) -
                    sqrt(AO_QUADRADO(secondCell->northFaceU) + AO_QUADRADO(secondCell->northFaceV)) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(secondCell->northFaceU) + AO_QUADRADO(secondCell->northFaceV)));
                        // northFaceMach
    }

    // Testa se a face oeste e' maior
    if( secondCell->pAtual != 0.0 )
    {
        if ( fabs( (secondCell->pAtual - secondCell->westFaceP) / secondCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (secondCell->pAtual - secondCell->westFaceP) / secondCell->pAtual );
    }
    else
    {
        if ( fabs( secondCell->pAtual - secondCell->westFaceP ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( secondCell->pAtual - secondCell->westFaceP );
    }

    if( secondCell->TAtual != 0.0 )
    {
        if ( fabs( (secondCell->TAtual - secondCell->westFaceT) / secondCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (secondCell->TAtual - secondCell->westFaceT) / secondCell->TAtual );
    }
    else
    {
        if ( fabs( secondCell->TAtual - secondCell->westFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( secondCell->TAtual - secondCell->westFaceT );
    }

    if( sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) -
                    sqrt(AO_QUADRADO(secondCell->westFaceU) + AO_QUADRADO(secondCell->westFaceV) ) )
                    / sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(secondCell->westFaceU) + AO_QUADRADO(secondCell->westFaceV)))
                        // westFaceMach
                / (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) -
                    sqrt(AO_QUADRADO(secondCell->westFaceU) + AO_QUADRADO(secondCell->westFaceV) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(secondCell->westFaceU) + AO_QUADRADO(secondCell->westFaceV)));
                        // westFaceMach
    }

    // Testa se a face sul e' maior
    if( secondCell->pAtual != 0.0 )
    {
        if ( fabs( (secondCell->pAtual - secondCell->southFaceP) / secondCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (secondCell->pAtual - secondCell->southFaceP) / secondCell->pAtual );
    }
    else
    {
        if ( fabs( secondCell->pAtual - secondCell->southFaceP ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( secondCell->pAtual - secondCell->southFaceP );
    }

    if( secondCell->TAtual != 0.0 )
    {
        if ( fabs( (secondCell->TAtual - secondCell->southFaceT) / secondCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (secondCell->TAtual - secondCell->southFaceT) / secondCell->TAtual );
    }
    else
    {
        if ( fabs( secondCell->TAtual - secondCell->southFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( secondCell->TAtual - secondCell->southFaceT );
    }

    if( sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) -
                    sqrt(AO_QUADRADO(secondCell->southFaceU) + AO_QUADRADO(secondCell->southFaceV) ) )
                    / sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(secondCell->southFaceU) + AO_QUADRADO(secondCell->southFaceV)))
                        // westFaceMach
                / (sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual) ) -
                    sqrt(AO_QUADRADO(secondCell->southFaceU) + AO_QUADRADO(secondCell->southFaceV) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(secondCell->uAtual) + AO_QUADRADO(secondCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(secondCell->southFaceU) + AO_QUADRADO(secondCell->southFaceV)));
                        // westFaceMach
    }

    maximum2_p = maximumFaceDifferenceP;
    maximum2_T = maximumFaceDifferenceT;
    maximum2_mach = maximumFaceDifferenceMach;

    //============================================================================================
    // maximum3 e' o maximo de p entre as faces do thirdCell
    // Assume que o maior e' o do leste
    if( firstCell->pAtual != 0.0 )
        maximumFaceDifferenceP = fabs(
                        (thirdCell->pAtual - thirdCell->eastFaceP) / thirdCell->pAtual
                    );
    else
        maximumFaceDifferenceP = fabs( thirdCell->pAtual - thirdCell->eastFaceP );

    if( thirdCell->TAtual != 0.0 )
        maximumFaceDifferenceT = fabs(
                        (thirdCell->TAtual - thirdCell->eastFaceT) / thirdCell->TAtual
                    );
    else
        maximumFaceDifferenceT = fabs( thirdCell->TAtual - thirdCell->eastFaceT );

    if( sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) != 0.0 )
        maximumFaceDifferenceMach = fabs(
        (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) -     // machAtual
        sqrt(AO_QUADRADO(thirdCell->eastFaceU) + AO_QUADRADO(thirdCell->eastFaceV)))    // eastFaceMach
        / (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)))
                    );
    else
        maximumFaceDifferenceMach = fabs(
        sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) -      // machAtual
        sqrt(AO_QUADRADO(thirdCell->eastFaceU) + AO_QUADRADO(thirdCell->eastFaceV)));   // eastFaceMach

    // Testa se a face norte e' maior
    if( thirdCell->pAtual != 0.0 )
    {
        if( fabs( (thirdCell->pAtual - thirdCell->northFaceP) / thirdCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (thirdCell->pAtual - thirdCell->northFaceP) / thirdCell->pAtual );
    }
    else
    {
        if( fabs( (thirdCell->pAtual - thirdCell->northFaceP) ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( thirdCell->pAtual - thirdCell->northFaceP );
    }

    if( thirdCell->TAtual != 0.0 )
    {
        if ( fabs( (thirdCell->TAtual - thirdCell->northFaceT) / thirdCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (thirdCell->TAtual - thirdCell->northFaceT) / thirdCell->TAtual );
    }
    else
    {
        if ( fabs( thirdCell->TAtual - thirdCell->northFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( thirdCell->TAtual - thirdCell->northFaceT );
    }


    if( sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) -
                    sqrt(AO_QUADRADO(thirdCell->northFaceU) + AO_QUADRADO(thirdCell->northFaceV) ) )
                    / sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(thirdCell->northFaceU) + AO_QUADRADO(thirdCell->northFaceV)))
                        // northFaceMach
                / (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) -
                    sqrt(AO_QUADRADO(thirdCell->northFaceU) + AO_QUADRADO(thirdCell->northFaceV)) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(thirdCell->northFaceU) + AO_QUADRADO(thirdCell->northFaceV)));
                        // northFaceMach
    }

    // Testa se a face oeste e' maior
    if( thirdCell->pAtual != 0.0 )
    {
        if ( fabs( (thirdCell->pAtual - thirdCell->westFaceP) / thirdCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (thirdCell->pAtual - thirdCell->westFaceP) / thirdCell->pAtual );
    }
    else
    {
        if ( fabs( thirdCell->pAtual - thirdCell->westFaceP ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( thirdCell->pAtual - thirdCell->westFaceP );
    }

    if( thirdCell->TAtual != 0.0 )
    {
        if ( fabs( (thirdCell->TAtual - thirdCell->westFaceT) / thirdCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (thirdCell->TAtual - thirdCell->westFaceT) / thirdCell->TAtual );
    }
    else
    {
        if ( fabs( thirdCell->TAtual - thirdCell->westFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( thirdCell->TAtual - thirdCell->westFaceT );
    }

    if( sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) -
                    sqrt(AO_QUADRADO(thirdCell->westFaceU) + AO_QUADRADO(thirdCell->westFaceV) ) )
                    / sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(thirdCell->westFaceU) + AO_QUADRADO(thirdCell->westFaceV)))
                        // westFaceMach
                / (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) -
                    sqrt(AO_QUADRADO(thirdCell->westFaceU) + AO_QUADRADO(thirdCell->westFaceV) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(thirdCell->westFaceU) + AO_QUADRADO(thirdCell->westFaceV)));
                        // westFaceMach
    }

    // Testa se a face sul e' maior
    if( thirdCell->pAtual != 0.0 )
    {
        if ( fabs( (thirdCell->pAtual - thirdCell->southFaceP) / thirdCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (thirdCell->pAtual - thirdCell->southFaceP) / thirdCell->pAtual );
    }
    else
    {
        if ( fabs( thirdCell->pAtual - thirdCell->southFaceP ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( thirdCell->pAtual - thirdCell->southFaceP );
    }

    if( thirdCell->TAtual != 0.0 )
    {
        if ( fabs( (thirdCell->TAtual - thirdCell->southFaceT) / thirdCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (thirdCell->TAtual - thirdCell->southFaceT) / thirdCell->TAtual );
    }
    else
    {
        if ( fabs( thirdCell->TAtual - thirdCell->southFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( thirdCell->TAtual - thirdCell->southFaceT );
    }

    if( sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) -
                    sqrt(AO_QUADRADO(thirdCell->southFaceU) + AO_QUADRADO(thirdCell->southFaceV) ) )
                    / sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(thirdCell->southFaceU) + AO_QUADRADO(thirdCell->southFaceV)))
                        // westFaceMach
                / (sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual) ) -
                    sqrt(AO_QUADRADO(thirdCell->southFaceU) + AO_QUADRADO(thirdCell->southFaceV) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(thirdCell->uAtual) + AO_QUADRADO(thirdCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(thirdCell->southFaceU) + AO_QUADRADO(thirdCell->southFaceV)));
                        // westFaceMach
    }


    maximum3_p = maximumFaceDifferenceP;
    maximum3_T = maximumFaceDifferenceT;
    maximum3_mach = maximumFaceDifferenceMach;      
    
    //============================================================================================
    // maximum4 e' o maximo de p entre as faces do fourthCell
    // Assume que o maior e' o do leste
    if( fourthCell->pAtual != 0.0 )
        maximumFaceDifferenceP = fabs(
                        (fourthCell->pAtual - fourthCell->eastFaceP) / fourthCell->pAtual
                    );
    else
        maximumFaceDifferenceP = fabs( fourthCell->pAtual - fourthCell->eastFaceP );

    if( fourthCell->TAtual != 0.0 )
        maximumFaceDifferenceT = fabs(
                        (fourthCell->TAtual - fourthCell->eastFaceT) / fourthCell->TAtual
                    );
    else
        maximumFaceDifferenceT = fabs( fourthCell->TAtual - fourthCell->eastFaceT );

    if( sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) != 0.0 )
        maximumFaceDifferenceMach = fabs(
        (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) -     // machAtual
        sqrt(AO_QUADRADO(fourthCell->eastFaceU) + AO_QUADRADO(fourthCell->eastFaceV)))    // eastFaceMach
        / (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)))
                    );
    else
        maximumFaceDifferenceMach = fabs(
        sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) -      // machAtual
        sqrt(AO_QUADRADO(fourthCell->eastFaceU) + AO_QUADRADO(fourthCell->eastFaceV)));   // eastFaceMach

    // Testa se a face norte e' maior
    if( fourthCell->pAtual != 0.0 )
    {
        if( fabs( (fourthCell->pAtual - fourthCell->northFaceP) / fourthCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (fourthCell->pAtual - fourthCell->northFaceP) / fourthCell->pAtual );
    }
    else
    {
        if( fabs( (fourthCell->pAtual - fourthCell->northFaceP) ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( fourthCell->pAtual - fourthCell->northFaceP );
    }

    if( fourthCell->TAtual != 0.0 )
    {
        if ( fabs( (fourthCell->TAtual - fourthCell->northFaceT) / fourthCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (fourthCell->TAtual - fourthCell->northFaceT) / fourthCell->TAtual );
    }
    else
    {
        if ( fabs( fourthCell->TAtual - fourthCell->northFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( fourthCell->TAtual - fourthCell->northFaceT );
    }


    if( sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) -
                    sqrt(AO_QUADRADO(fourthCell->northFaceU) + AO_QUADRADO(fourthCell->northFaceV) ) )
                    / sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(fourthCell->northFaceU) + AO_QUADRADO(fourthCell->northFaceV)))
                        // northFaceMach
                / (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) -
                    sqrt(AO_QUADRADO(fourthCell->northFaceU) + AO_QUADRADO(fourthCell->northFaceV)) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(fourthCell->northFaceU) + AO_QUADRADO(fourthCell->northFaceV)));
                        // northFaceMach
    }

    // Testa se a face oeste e' maior
    if( fourthCell->pAtual != 0.0 )
    {
        if ( fabs( (fourthCell->pAtual - fourthCell->westFaceP) / fourthCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (fourthCell->pAtual - fourthCell->westFaceP) / fourthCell->pAtual );
    }
    else
    {
        if ( fabs( fourthCell->pAtual - fourthCell->westFaceP ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( fourthCell->pAtual - fourthCell->westFaceP );
    }

    if( fourthCell->TAtual != 0.0 )
    {
        if ( fabs( (fourthCell->TAtual - fourthCell->westFaceT) / fourthCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (fourthCell->TAtual - fourthCell->westFaceT) / fourthCell->TAtual );
    }
    else
    {
        if ( fabs( fourthCell->TAtual - fourthCell->westFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( fourthCell->TAtual - fourthCell->westFaceT );
    }

    if( sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) -
                    sqrt(AO_QUADRADO(fourthCell->westFaceU) + AO_QUADRADO(fourthCell->westFaceV) ) )
                    / sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(fourthCell->westFaceU) + AO_QUADRADO(fourthCell->westFaceV)))
                        // westFaceMach
                / (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) -
                    sqrt(AO_QUADRADO(fourthCell->westFaceU) + AO_QUADRADO(fourthCell->westFaceV) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(fourthCell->westFaceU) + AO_QUADRADO(fourthCell->westFaceV)));
                        // westFaceMach
    }

    // Testa se a face sul e' maior
    if( fourthCell->pAtual != 0.0 )
    {
        if ( fabs( (fourthCell->pAtual - fourthCell->southFaceP) / fourthCell->pAtual )
            > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( (fourthCell->pAtual - fourthCell->southFaceP) / fourthCell->pAtual );
    }
    else
    {
        if ( fabs( fourthCell->pAtual - fourthCell->southFaceP ) > maximumFaceDifferenceP )
            maximumFaceDifferenceP = fabs( fourthCell->pAtual - fourthCell->southFaceP );
    }

    if( fourthCell->TAtual != 0.0 )
    {
        if ( fabs( (fourthCell->TAtual - fourthCell->southFaceT) / fourthCell->TAtual )
            > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( (fourthCell->TAtual - fourthCell->southFaceT) / fourthCell->TAtual );
    }
    else
    {
        if ( fabs( fourthCell->TAtual - fourthCell->southFaceT ) > maximumFaceDifferenceT )
            maximumFaceDifferenceT = fabs( fourthCell->TAtual - fourthCell->southFaceT );
    }

    if( sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) != 0.0 )
    {
        if ( fabs( (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) -
                    sqrt(AO_QUADRADO(fourthCell->southFaceU) + AO_QUADRADO(fourthCell->southFaceV) ) )
                    / sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(fourthCell->southFaceU) + AO_QUADRADO(fourthCell->southFaceV)))
                        // westFaceMach
                / (sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)))
                        );
    }
    else
    {
        if ( fabs( sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual) ) -
                    sqrt(AO_QUADRADO(fourthCell->southFaceU) + AO_QUADRADO(fourthCell->southFaceV) ) )
            > maximumFaceDifferenceMach )
            maximumFaceDifferenceMach = fabs(
                sqrt(AO_QUADRADO(fourthCell->uAtual) + AO_QUADRADO(fourthCell->vAtual)) -     // machAtual
                sqrt(AO_QUADRADO(fourthCell->southFaceU) + AO_QUADRADO(fourthCell->southFaceV)));
                        // westFaceMach
    }


    maximum4_p = maximumFaceDifferenceP;
    maximum4_T = maximumFaceDifferenceT;
    maximum4_mach = maximumFaceDifferenceMach;
        
    // Acha o maximo entre os maximos:

    highestMaximum_p = maximum1_p;
    if( maximum2_p > highestMaximum_p ) 
        highestMaximum_p = maximum2_p;
    if( maximum3_p > highestMaximum_p ) 
        highestMaximum_p = maximum3_p;
    if( maximum4_p > highestMaximum_p ) 
        highestMaximum_p = maximum4_p;

    highestMaximum_T = maximum1_T;
    if( maximum2_T > highestMaximum_T ) 
        highestMaximum_T = maximum2_T;
    if( maximum3_T > highestMaximum_T ) 
        highestMaximum_T = maximum3_T;
    if( maximum4_T > highestMaximum_T ) 
        highestMaximum_T = maximum4_T;

    highestMaximum_mach = maximum1_mach;
    if( maximum2_mach > highestMaximum_mach ) 
        highestMaximum_mach = maximum2_mach;
    if( maximum3_mach > highestMaximum_mach ) 
        highestMaximum_mach = maximum3_mach;
    if( maximum4_mach > highestMaximum_mach ) 
        highestMaximum_mach = maximum4_mach;
       
    if( highestMaximum_p <= derefinementBound
    &&  highestMaximum_T <= derefinementBound
    &&  highestMaximum_mach <= derefinementBound )
        derefinementCondition = true;
        
    return derefinementCondition;
            
}//end needsDerefinement() function

//end program derefinement.cpp
