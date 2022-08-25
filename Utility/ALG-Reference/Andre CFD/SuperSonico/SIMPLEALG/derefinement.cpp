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
    while( gridCell != 0 )
    {
        hasBeenDerefined = false;
        if( gridCell->level > grid->refinamentoMinimo )
        {
		 	// Buscar cachos
            if( (gridCell->next != 0) 
            &&  (gridCell->next->next != 0) 
            &&  (gridCell->next->next->next != 0) )
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
           NOVOrhoIterAnterior,
           NOVOuAtual,
           NOVOuAnterior,
           NOVOuIterAnterior,
           NOVOvAtual,
           NOVOvAnterior,
           NOVOvIterAnterior,
           NOVOEtAtual,
           NOVOEtAnterior,
           NOVOEtIterAnterior,
           NOVOTAtual,
           NOVOTAnterior,
           NOVOTIterAnterior,
           NOVOpAtual,
           NOVOpAnterior,
           NOVOpIterAnterior,
           NOVOmiAtual,
           NOVOmiAnterior,
           NOVOmiIterAnterior,
           NOVOkAtual,
           NOVOkAnterior,
           NOVOkIterAnterior;
          
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
    NOVOvIterAnterior = 
        (   firstCell->vIterAnterior + secondCell->vIterAnterior 
          + thirdCell->vIterAnterior + fourthCell->vIterAnterior 
        ) /4;
    NOVOEtAtual = 
        (   firstCell->EtAtual + secondCell->EtAtual 
          + thirdCell->EtAtual + fourthCell->EtAtual 
        ) /4;
    NOVOEtAnterior = 
        (   firstCell->EtAnterior + secondCell->EtAnterior 
          + thirdCell->EtAnterior + fourthCell->EtAnterior 
        ) /4;
    NOVOEtIterAnterior = 
        (   firstCell->EtIterAnterior + secondCell->EtIterAnterior 
          + thirdCell->EtIterAnterior + fourthCell->EtIterAnterior 
        ) /4;
    NOVOTAtual = 
        (   firstCell->TAtual + secondCell->TAtual 
          + thirdCell->TAtual + fourthCell->TAtual 
        ) /4;
    NOVOTAnterior = 
        (   firstCell->TAnterior + secondCell->TAnterior 
          + thirdCell->TAnterior + fourthCell->TAnterior 
        ) /4;
    NOVOTIterAnterior = 
        (   firstCell->TIterAnterior + secondCell->TIterAnterior 
          + thirdCell->TIterAnterior + fourthCell->TIterAnterior 
        ) /4;
    NOVOpAtual = 
        (   firstCell->pAtual + secondCell->pAtual 
          + thirdCell->pAtual + fourthCell->pAtual 
        ) /4;
    NOVOpAnterior = 
        (   firstCell->pAnterior + secondCell->pAnterior 
          + thirdCell->pAnterior + fourthCell->pAnterior 
        ) /4;
    NOVOpIterAnterior = 
        (   firstCell->pIterAnterior + secondCell->pIterAnterior 
          + thirdCell->pIterAnterior + fourthCell->pIterAnterior 
        ) /4;
    NOVOmiAtual = 
        (   firstCell->miAtual + secondCell->miAtual 
          + thirdCell->miAtual + fourthCell->miAtual 
        ) /4;
    NOVOmiAnterior = 
        (   firstCell->miAnterior + secondCell->miAnterior 
          + thirdCell->miAnterior + fourthCell->miAnterior 
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
    NOVOkIterAnterior = 
        (   firstCell->pIterAnterior + secondCell->pIterAnterior 
          + thirdCell->pIterAnterior + fourthCell->pIterAnterior 
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
    newCell->rhoIterAnterior = NOVOrhoIterAnterior;
    newCell->uAtual = NOVOuAtual;
    newCell->uAnterior = NOVOuAnterior;
    newCell->uIterAnterior = NOVOuIterAnterior;
    newCell->vAtual = NOVOvAtual;
    newCell->vAnterior = NOVOvAnterior;
    newCell->vIterAnterior = NOVOvIterAnterior;
    newCell->EtAtual = NOVOEtAtual;
    newCell->EtAnterior = NOVOEtAnterior;
    newCell->EtIterAnterior = NOVOEtIterAnterior;
    newCell->TAtual = NOVOTAtual;
    newCell->TAnterior = NOVOTAnterior;
    newCell->TIterAnterior = NOVOTIterAnterior;
    newCell->pAtual = NOVOpAtual;
    newCell->pAnterior = NOVOpAnterior;
    newCell->pIterAnterior = NOVOpIterAnterior;
    newCell->miAtual = NOVOmiAtual;
    newCell->miAnterior = NOVOmiAnterior;
    newCell->miIterAnterior = NOVOmiIterAnterior;
    newCell->kAtual = NOVOkAtual;
    newCell->kAnterior = NOVOkAnterior;
    newCell->kIterAnterior = NOVOkIterAnterior;
    
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
    double maximum1,
           maximum2,
           maximum3,
           maximum4,
           highestMaximum;
    bool derefinementCondition = false;
         
    firstCell = gridCell;
    secondCell = gridCell->next,
    thirdCell = gridCell->next->next,
    fourthCell = gridCell->next->next->next;
    
    maximum1 = fabs(firstCell->pIterAnterior - firstCell->eastFaceP);
    if( fabs(firstCell->pIterAnterior - firstCell->northFaceP) > maximum1 ) 
        maximum1 = fabs(firstCell->pIterAnterior - firstCell->northFaceP);
    if( fabs(firstCell->pIterAnterior - firstCell->westFaceP) > maximum1 ) 
        maximum1 = fabs(firstCell->pIterAnterior - firstCell->westFaceP);
    if( fabs(firstCell->pIterAnterior - firstCell->southFaceP) > maximum1 ) 
        maximum1 = fabs(firstCell->pIterAnterior - firstCell->southFaceP);        
    
    maximum2 = fabs(secondCell->pIterAnterior - secondCell->eastFaceP);
    if( fabs(secondCell->pIterAnterior - secondCell->northFaceP) > maximum2 ) 
        maximum2 = fabs(secondCell->pIterAnterior - secondCell->northFaceP);
    if( fabs(secondCell->pIterAnterior - secondCell->westFaceP) > maximum2 ) 
        maximum2 = fabs(secondCell->pIterAnterior - secondCell->westFaceP);
    if( fabs(secondCell->pIterAnterior - secondCell->southFaceP) > maximum2 ) 
        maximum2 = fabs(secondCell->pIterAnterior - secondCell->southFaceP);
    
    maximum3 = fabs(thirdCell->pIterAnterior - thirdCell->eastFaceP);
    if( fabs(thirdCell->pIterAnterior - thirdCell->northFaceP) > maximum3 ) 
        maximum3 = fabs(thirdCell->pIterAnterior - thirdCell->northFaceP);
    if( fabs(thirdCell->pIterAnterior - thirdCell->westFaceP) > maximum3 ) 
        maximum3 = fabs(thirdCell->pIterAnterior - thirdCell->westFaceP);
    if( fabs(thirdCell->pIterAnterior - thirdCell->southFaceP) > maximum3 ) 
        maximum3 = fabs(thirdCell->pIterAnterior - thirdCell->southFaceP);
    
    maximum4 = fabs(fourthCell->pIterAnterior - fourthCell->eastFaceP);
    if( fabs(fourthCell->pIterAnterior - fourthCell->northFaceP) > maximum4 ) 
        maximum4 = fabs(fourthCell->pIterAnterior - fourthCell->northFaceP);
    if( fabs(fourthCell->pIterAnterior - fourthCell->westFaceP) > maximum4 ) 
        maximum4 = fabs(fourthCell->pIterAnterior - fourthCell->westFaceP);
    if( fabs(fourthCell->pIterAnterior - fourthCell->southFaceP) > maximum4 ) 
        maximum4 = fabs(fourthCell->pIterAnterior - fourthCell->southFaceP);
        
    highestMaximum = maximum1;
    if( maximum2 > highestMaximum ) 
        highestMaximum = maximum2;
    if( maximum3 > highestMaximum ) 
        highestMaximum = maximum3;
    if( maximum4 > highestMaximum ) 
        highestMaximum = maximum4;
    
    if( highestMaximum <= derefinementBound )
        derefinementCondition = true;
        
    return derefinementCondition;
            
}//end needsDerefinement() function

//end program derefinement.cpp
