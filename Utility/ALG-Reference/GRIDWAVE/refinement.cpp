//refinement.cpp
//Refinement functions.

#include "Cell.h"
#include "Grid.h"
#include "functions.h"

#include <cmath>
    
#include <fstream>
    using std::ofstream;
    
#include <iostream>
    using std::cout;
    using std::endl;
    using std::ios;

    
/*______________________________________________________________________________
________________________________________________________________________________

                        FUNCTION refineGrid()
                        
    Decides if the grid should be refined by traversing the whole grid, according 
    to the parameters refinementLevel and refinementBound.
    A cell will not be refined either if its level of refinement is equal to
    refinementLevel or the highest of all fluxes coming into it from the four
    directions is less than refinementBound.
    
________________________________________________________________________________
______________________________________________________________________________*/
void refineGrid( Grid *grid, int refinementLevel, double refinementBound, 
                 bool drawGrid )
{
    Cell *gridCell,
    	 *auxiliarGridCell;
    double maximumFlux;
    bool continueRefining = true;
    
    while( continueRefining )
    {
        continueRefining = false;
        
        setGridFlux( grid );
        
        gridCell = grid->firstGridCell;   
        while( gridCell != 0 )
        {
            //Computes maximum flux coming to this cell.	   
            maximumFlux = fabs( gridCell->eastFlux );
            if ( fabs( gridCell->northFlux ) > maximumFlux )
                 maximumFlux = fabs( gridCell->northFlux );
            if ( fabs( gridCell->westFlux ) > maximumFlux )
                 maximumFlux = fabs( gridCell->westFlux );
            if ( fabs( gridCell->southFlux ) > maximumFlux )
                 maximumFlux = fabs( gridCell->southFlux );
                        
            if( ( gridCell->level < refinementLevel ) && 
                ( maximumFlux >= refinementBound ) )
            {                
                auxiliarGridCell = gridCell;
                gridCell = gridCell->next;
				grid->orderGridCells();
                grid->drawHilbertCurve(1);
                refineCell( auxiliarGridCell, drawGrid );
                grid->numberOfCells += 3;
                
                continueRefining = true;
            }
            else
                gridCell = gridCell->next;
        }
    }    
    
    grid->orderGridCells();
}//end refineGrid() function.

/*______________________________________________________________________________
________________________________________________________________________________
            				FUNCTION refineCell()
            					
    Refines a cell, tranforming it into a bunch with four cell nodes and four
	transition nodes with the following shape:
    								
    								 w
            					   /   \
                                  b --- b
                                / |     | \
                               w  |     |  w
                                \ |     | /
                                  b --- b
                                   \   /
                                     w

________________________________________________________________________________
______________________________________________________________________________*/
void refineCell( Cell *cell, bool drawGrid )
{
    Cell *eastTransitionNode,
         *northTransitionNode,
         *westTransitionNode,
         *southTransitionNode;

    Cell *northeastSubCell,
  		 *northwestSubCell,
         *southwestSubCell,
         *southeastSubCell;

    int j, 
        squareOfJ,
        cellHilbertShapeNumber,
        cellRefinementLevel,
        numberOfHilbertShape;

    double cellCenterX = cell->centerX,
  		   cellCenterY = cell->centerY,
  		   cellHalfSide = cell->halfFaceLength,
  		   cellQuarterSide = cell->halfFaceLength / 2.0;

    if( drawGrid )
    {
        //Erases the portion of the Hilbert curve in this cell and in the previous
        //and next cells.
   	    drawHilbertCurveLocally( cell, 7, false );
   	    
   	    //Draws "+" in the center of the cell.
   	    setColor(5);
   	    drawLine( cellCenterX - cellHalfSide, cellCenterY,
   	    		  cellCenterX + cellHalfSide, cellCenterY );
	    drawLine( cellCenterX, cellCenterY - cellHalfSide,
 			      cellCenterX, cellCenterY + cellHalfSide );
     }

    j = 1;
    for( int i = 0; i < cell->level; i++ )
    {
        j *= 2;
    }
    squareOfJ = j*j;

    /*__________________________________________________________________________
    						CREATION OF THE NORTHEAST CELL
    __________________________________________________________________________*/
    /* This cell, which is to be refined, becomes the northeast cell of the new bunch. */
    northeastSubCell = cell;

    northeastSubCell->level = cell->level + 1;
 	northeastSubCell->bunchNumber = (northeastSubCell->bunchNumber)*10  + 1;
 	northeastSubCell->faceLength = cellHalfSide;
 	northeastSubCell->halfFaceLength = cellQuarterSide;
    northeastSubCell->centerX = cellCenterX + cellQuarterSide;
    northeastSubCell->centerY = cellCenterY + cellQuarterSide;


 	/*__________________________________________________________________________
                            CREATION OF TRANSITION NODES
    __________________________________________________________________________*/
 	/* Creates 4 transition nodes around the cell.
                              w
                              |
                          w --b-- w
                              |
                              w 
    */
 	/* West transition node. */
 	westTransitionNode = new Cell;
  	westTransitionNode->type = 'w';
  	westTransitionNode->level = northeastSubCell->level;
  	westTransitionNode->active = northeastSubCell->active;
  	westTransitionNode->singleConnector = northeastSubCell->west;
  	/* Points the west neighboring cell to the transition node.
                              w
                              |
                    b -- w -- b -- w
                              |
                              w 
    */
    if( northeastSubCell->west->type == 'b' )
        northeastSubCell->west->east = westTransitionNode;
    if( northeastSubCell->west->type == 'w' )
    {
        if( northeastSubCell->west->singleConnector == northeastSubCell )
            northeastSubCell->west->singleConnector = westTransitionNode;
        else if( northeastSubCell->west->doubleConnector1 == northeastSubCell )
                 northeastSubCell->west->doubleConnector1 = westTransitionNode;
        else if( northeastSubCell->west->doubleConnector2 == northeastSubCell )
                 northeastSubCell->west->doubleConnector2 = westTransitionNode;         
    }
    /* Points this cell to the transition node. */
    northeastSubCell->west = westTransitionNode;
    
    /* North transition node. */
 	northTransitionNode = new Cell;
  	northTransitionNode->type = 'w';
  	northTransitionNode->level = northeastSubCell->level;
  	northTransitionNode->active = northeastSubCell->active;
  	northTransitionNode->singleConnector = northeastSubCell->north;
  	/* Points the north neighboring cell to the transition node.
  	                          b
  	                          |
                              w 
                              |
                         w -- b -- w 
                              |
                              w 
    */
    if( northeastSubCell->north->type == 'b' )
        northeastSubCell->north->south = northTransitionNode;
    if( northeastSubCell->north->type == 'w' )
    {
        if( northeastSubCell->north->singleConnector == northeastSubCell )
            northeastSubCell->north->singleConnector = northTransitionNode;
        else if( northeastSubCell->north->doubleConnector1 == northeastSubCell )
                 northeastSubCell->north->doubleConnector1 = northTransitionNode;
        else if( northeastSubCell->north->doubleConnector2 == northeastSubCell )
                 northeastSubCell->north->doubleConnector2 = northTransitionNode;         
    }
    /* Points this cell to the transition node. */
    northeastSubCell->north = northTransitionNode;
    
    /* South transition node. */
 	southTransitionNode = new Cell;
  	southTransitionNode->type = 'w';
  	southTransitionNode->level = northeastSubCell->level;
  	southTransitionNode->active = northeastSubCell->active;
  	southTransitionNode->singleConnector = northeastSubCell->south;
  	/* Points the south neighboring cell to the transition node.
  	                          w
                              |
                         w -- b -- w
                              |
                              w 
                              |
                              b
    */
    if( northeastSubCell->south->type == 'b' )
        northeastSubCell->south->north = southTransitionNode;
    if( northeastSubCell->south->type == 'w' )
    {
        if( northeastSubCell->south->singleConnector == northeastSubCell )
            northeastSubCell->south->singleConnector = southTransitionNode;
        else if( northeastSubCell->south->doubleConnector1 == northeastSubCell )
                 northeastSubCell->south->doubleConnector1 = southTransitionNode;
        else if( northeastSubCell->south->doubleConnector2 == northeastSubCell )
                 northeastSubCell->south->doubleConnector2 = southTransitionNode;         
    }
    /* Points this cell to the transition node. */
    northeastSubCell->south = southTransitionNode;
    
    /* East transition node. */
 	eastTransitionNode = new Cell;
  	eastTransitionNode->type = 'w';
  	eastTransitionNode->level = northeastSubCell->level;
  	eastTransitionNode->active = northeastSubCell->active;
  	eastTransitionNode->singleConnector = northeastSubCell->east;
  	/* Points the east neighboring cell to the transition node.
  	                          w
                              |
                         w -- b -- w -- b
                              |
                              w 
    */
    if( northeastSubCell->east->type == 'b' )
        northeastSubCell->east->west = eastTransitionNode;
    if( northeastSubCell->east->type == 'w' )
    {
        if( northeastSubCell->east->singleConnector == northeastSubCell )
            northeastSubCell->east->singleConnector = eastTransitionNode;
        else if( northeastSubCell->east->doubleConnector1 == northeastSubCell )
                 northeastSubCell->east->doubleConnector1 = eastTransitionNode;
        else if( northeastSubCell->east->doubleConnector2 == northeastSubCell )
                 northeastSubCell->east->doubleConnector2 = eastTransitionNode;         
    }
    /* Points this cell to the transition node. */
    northeastSubCell->east = eastTransitionNode;

    /*__________________________________________________________________________
                            CREATION OF 3 NEW CELL NODES
    __________________________________________________________________________*/
    /* Creation of northwest node. */
 	northwestSubCell = new Cell;
   	northwestSubCell->type = 'b';
   	northwestSubCell->level = northeastSubCell->level;
    northwestSubCell->active = northeastSubCell->active;
    northwestSubCell->bunchNumber = northeastSubCell->bunchNumber + 1;
 	northwestSubCell->faceLength = cellHalfSide;
 	northwestSubCell->halfFaceLength = cellQuarterSide;
    northwestSubCell->centerX = cellCenterX - cellQuarterSide;    
    northwestSubCell->centerY = cellCenterY + cellQuarterSide;
    northwestSubCell->u = northeastSubCell->u;
    northwestSubCell->previousU = northeastSubCell->previousU;
    
    /* Creation of southwest node. */
 	southwestSubCell = new Cell;
   	southwestSubCell->type = 'b';
   	southwestSubCell->level = northeastSubCell->level;
    southwestSubCell->active = northeastSubCell->active;
    southwestSubCell->bunchNumber = northeastSubCell->bunchNumber + 2;
 	southwestSubCell->faceLength = cellHalfSide;
 	southwestSubCell->halfFaceLength = cellQuarterSide;
    southwestSubCell->centerX = cellCenterX - cellQuarterSide;    
    southwestSubCell->centerY = cellCenterY - cellQuarterSide;
    southwestSubCell->u = northeastSubCell->u;
    southwestSubCell->previousU = northeastSubCell->previousU;

    /* Creation of southeast node. */
 	southeastSubCell = new Cell;
   	southeastSubCell->type = 'b';
   	southeastSubCell->level = northeastSubCell->level;
    southeastSubCell->active = northeastSubCell->active;
    southeastSubCell->bunchNumber = northeastSubCell->bunchNumber + 3;
 	southeastSubCell->faceLength = cellHalfSide;
 	southeastSubCell->halfFaceLength = cellQuarterSide;
    southeastSubCell->centerX = cellCenterX + cellQuarterSide;    
    southeastSubCell->centerY = cellCenterY - cellQuarterSide;
    southeastSubCell->u = northeastSubCell->u;
    southeastSubCell->previousU = northeastSubCell->previousU;
    
    /*__________________________________________________________________________
                       LINKING OF NEW CELL AND TRANSITION NODES
    __________________________________________________________________________*/
    /* Connects the new 4 cell nodes.
                        NW - NE
                        |     |
                        SW - SE
    */
    northwestSubCell->north = northeastSubCell->north;    
    northwestSubCell->west = northeastSubCell->west;
    northwestSubCell->east = northeastSubCell;
    northwestSubCell->south = southwestSubCell;
    
    southwestSubCell->west = northeastSubCell->west;
    southwestSubCell->south = northeastSubCell->south;
    southwestSubCell->north = northwestSubCell;
    southwestSubCell->east = southeastSubCell;
    
    southeastSubCell->south = northeastSubCell->south;
    southeastSubCell->east = northeastSubCell->east;
    southeastSubCell->west = southwestSubCell;
    southeastSubCell->north = northeastSubCell;
        
    northeastSubCell->west = northwestSubCell;
    northeastSubCell->south = southeastSubCell;
    
    /* Connects the cell nodes with the transition nodes.
       Double connectors 1 and 2 are connected to neighbor cells in the way
       depicted below. 
       This choice is made consistent with the one made at the function
	   simplify(), so that when two transition nodes of same level connected
	   through their single connector are eliminated, the subsequent linking of
	   corresponding double connectors is correctly done.
    
                                     w
		              			 2 /   \ 1
                                  b --- b
                              1 / |     | \ 1
                               w  |     |  w
                              2 \ |     | / 2
                                  b --- b
                                2  \   / 1
                                     w

    */
    northTransitionNode->doubleConnector1 = northeastSubCell;
    northTransitionNode->doubleConnector2 = northwestSubCell; 
    
    westTransitionNode->doubleConnector1 = northwestSubCell;
    westTransitionNode->doubleConnector2 = southwestSubCell;
    
    southTransitionNode->doubleConnector1 = southeastSubCell;
    southTransitionNode->doubleConnector2 = southwestSubCell;
    
    eastTransitionNode->doubleConnector1 = northeastSubCell;
    eastTransitionNode->doubleConnector2 = southeastSubCell;
    
    
    /*__________________________________________________________________________
                ORDERING OF CELL NODES THROUGH HILBERT'S CURVE
    __________________________________________________________________________*/
    cellHilbertShapeNumber =  northeastSubCell->hilbertShapeNumber;
    cellRefinementLevel = northeastSubCell->level;
    numberOfHilbertShape = computeHilbertShapeNumber( cellHilbertShapeNumber, cellRefinementLevel );
    
    /*Shape
     								 ___        
			                        |      
           			                |___   
    */
    if( numberOfHilbertShape == 0 )
    {
        northwestSubCell->hilbertShapeNumber = cellHilbertShapeNumber + squareOfJ;
        southwestSubCell->hilbertShapeNumber = cellHilbertShapeNumber + 2 * squareOfJ;
        southeastSubCell->hilbertShapeNumber = cellHilbertShapeNumber + 3 * squareOfJ;

        northwestSubCell->next = southwestSubCell;
        southwestSubCell->next = southeastSubCell;
        southeastSubCell->next = northeastSubCell->next;
        northeastSubCell->next = northwestSubCell;

        northwestSubCell->previous = northeastSubCell;
        southwestSubCell->previous = northwestSubCell;
        southeastSubCell->previous = southwestSubCell;
        if( southeastSubCell->next != 0 )
        	southeastSubCell->next->previous = southeastSubCell;
        	
    }
    /*Shape
     								|   |
     								|___|        
    */
    else if( numberOfHilbertShape == 1 )
    {
        southeastSubCell->hilbertShapeNumber = cellHilbertShapeNumber + squareOfJ;
        southwestSubCell->hilbertShapeNumber = cellHilbertShapeNumber + 2 * squareOfJ;
        northwestSubCell->hilbertShapeNumber = cellHilbertShapeNumber + 3 * squareOfJ;

        southeastSubCell->next = southwestSubCell;
        southwestSubCell->next = northwestSubCell;
        northwestSubCell->next = northeastSubCell->next;
        northeastSubCell->next = southeastSubCell;
        
        southeastSubCell->previous = northeastSubCell;
        southwestSubCell->previous = southeastSubCell;
        northwestSubCell->previous = southwestSubCell;
        if( northwestSubCell->next != 0 )
        	northwestSubCell->next->previous = northwestSubCell;
    }
    /*Shape
     								___   
             						   | 
                      				___|       
    */
    else if( numberOfHilbertShape == 2 )
    {
        southwestSubCell->hilbertShapeNumber = cellHilbertShapeNumber;
        southeastSubCell->hilbertShapeNumber = cellHilbertShapeNumber + squareOfJ;
        northeastSubCell->hilbertShapeNumber = cellHilbertShapeNumber + 2 * squareOfJ;
        northwestSubCell->hilbertShapeNumber = cellHilbertShapeNumber + 3 * squareOfJ;
        
        if( northeastSubCell->previous != 0 )
        	northeastSubCell->previous->next = southwestSubCell;
        southwestSubCell->next = southeastSubCell;
        southeastSubCell->next = northeastSubCell;
        northwestSubCell->next = northeastSubCell->next;
        northeastSubCell->next = northwestSubCell;
        
        northwestSubCell->previous = northeastSubCell;
        southeastSubCell->previous = southwestSubCell;
        southwestSubCell->previous = northeastSubCell->previous;
        northeastSubCell->previous = southeastSubCell;
        if( northwestSubCell->next != 0 )
        	northwestSubCell->next->previous = northwestSubCell;
   }
    /*Shape
     								 ___       
			                        |   |
			                        |   |          
    */
   else if( numberOfHilbertShape == 3 )
   {
       southwestSubCell->hilbertShapeNumber = cellHilbertShapeNumber;
       northwestSubCell->hilbertShapeNumber = cellHilbertShapeNumber + squareOfJ;
       northeastSubCell->hilbertShapeNumber = cellHilbertShapeNumber + 2 * squareOfJ;
       southeastSubCell->hilbertShapeNumber = cellHilbertShapeNumber + 3 * squareOfJ;
       
       southwestSubCell->next = northwestSubCell;       
       northwestSubCell->next = northeastSubCell;
       southeastSubCell->next = northeastSubCell->next;
       northeastSubCell->previous->next = southwestSubCell;
    
       southwestSubCell->previous = northeastSubCell->previous;
       southwestSubCell->previous->next = southwestSubCell;
       
       southeastSubCell->previous = northeastSubCell;
       northwestSubCell->previous = southwestSubCell;
       if( northeastSubCell->next != 0 )
        	northeastSubCell->next->previous = southeastSubCell;
       northeastSubCell->next = southeastSubCell;
       northeastSubCell->previous = northwestSubCell;
   }

    /*__________________________________________________________________________
                    	SIMPLIFICATION OF TRANSITION NODES

    If necessary, simplifies the graph by eliminating adjacent transition nodes
    of same level connected through their singleConnectors.
    __________________________________________________________________________*/
    simplify( eastTransitionNode);
    simplify( northTransitionNode);
    simplify( westTransitionNode );
    simplify( southTransitionNode );

}//end refineCell() function

/*______________________________________________________________________________
________________________________________________________________________________
            			
						FUNCTION computeHilbertShapeNumber()
            					
            Returns 0, 1, 2 or 3, which determine the shape of the Hilbert curve
            in the bunch created when the cell is refined, i.e.,
             ___        
            |    = 0,  
            |___                
             
            |   | = 1,
            |___|                
             ___   
                | = 2,
             ___|         
             ___       
            |   | = 3.
            |   |          

________________________________________________________________________________           
______________________________________________________________________________*/
int computeHilbertShapeNumber( int cellHilbertShapeNumber, 
                               int cellRefinementLevel )
{
    int hilbertNumberTable[4][4];   
	
    hilbertNumberTable[0][0]=1;
    hilbertNumberTable[0][1]=0;
    hilbertNumberTable[0][2]=0;
    hilbertNumberTable[0][3]=3;
         
    hilbertNumberTable[1][0]=0;
    hilbertNumberTable[1][1]=1;
    hilbertNumberTable[1][2]=1;
    hilbertNumberTable[1][3]=2;
       
    hilbertNumberTable[2][0]=3;
    hilbertNumberTable[2][1]=2;
    hilbertNumberTable[2][2]=2;
    hilbertNumberTable[2][3]=1;
       
    hilbertNumberTable[3][0]=2;
    hilbertNumberTable[3][1]=3;
    hilbertNumberTable[3][2]=3;
    hilbertNumberTable[3][3]=0; 
    
    int i = 0,
        j;
    for( int k = 1; k < cellRefinementLevel; k++ )
    {
        j = cellHilbertShapeNumber%4;
        i = hilbertNumberTable[i][j];
        cellHilbertShapeNumber /= 4;
    }
    return i;
}//end computeHilbertShapeNumber() function

/*______________________________________________________________________________
________________________________________________________________________________
                    	
						FUNCTION simplify()
                    	
    Simplifies the data structure, eliminating adjacent transition nodes
    of same level.
    
    Before:
    			   1____		____1
    				    \      /
    				     w -- w
   				        /      \
                   2____        ____2
                    
    After:
        			________________
    				      
    				     
   				        
                    ________________
        
________________________________________________________________________________           
______________________________________________________________________________*/

void simplify( Cell *transitionNode )
{
    Cell *neighborTransitionNode;
    neighborTransitionNode = transitionNode->singleConnector;
    
    if( ( transitionNode->type == 'w' ) && ( neighborTransitionNode->type == 'w' ) )
    {
        if( transitionNode->level == neighborTransitionNode->level )
        {
            if( transitionNode->doubleConnector1->type == 'b' )
            {
                if( transitionNode == transitionNode->doubleConnector1->south )
            	    transitionNode->doubleConnector1->south 
                               = neighborTransitionNode->doubleConnector1;
                else if( transitionNode == transitionNode->doubleConnector1->north )
            		     transitionNode->doubleConnector1->north 
                               = neighborTransitionNode->doubleConnector1;
                else if( transitionNode == transitionNode->doubleConnector1->east )
            		     transitionNode->doubleConnector1->east 
                               = neighborTransitionNode->doubleConnector1;
                else if( transitionNode == transitionNode->doubleConnector1->west )
            		     transitionNode->doubleConnector1->west 
                               = neighborTransitionNode->doubleConnector1;
            }
            else if( transitionNode->doubleConnector1->type == 'w' )
            {
                if( transitionNode == transitionNode->doubleConnector1->singleConnector )
            	    transitionNode->doubleConnector1->singleConnector
                               = neighborTransitionNode->doubleConnector1;
                else if( transitionNode == transitionNode->doubleConnector1->doubleConnector1 )
            		     transitionNode->doubleConnector1->doubleConnector1
                               = neighborTransitionNode->doubleConnector1;
                else if( transitionNode == transitionNode->doubleConnector1->doubleConnector2 )
            		     transitionNode->doubleConnector1->doubleConnector2
                               = neighborTransitionNode->doubleConnector1;                
            }
            
            if( transitionNode->doubleConnector2->type == 'b' )
            {
                if( transitionNode == transitionNode->doubleConnector2->south )
                	transitionNode->doubleConnector2->south 
                                   = neighborTransitionNode->doubleConnector2;
                else if( transitionNode == transitionNode->doubleConnector2->north )
                		 transitionNode->doubleConnector2->north 
                                   = neighborTransitionNode->doubleConnector2;
                else if( transitionNode == transitionNode->doubleConnector2->east )
                		 transitionNode->doubleConnector2->east 
                                   = neighborTransitionNode->doubleConnector2;
                else if( transitionNode == transitionNode->doubleConnector2->west )
                		 transitionNode->doubleConnector2->west 
                                   = neighborTransitionNode->doubleConnector2;
            }
            else if( transitionNode->doubleConnector2->type == 'w' )
            {
                if( transitionNode == transitionNode->doubleConnector2->singleConnector )
            	    transitionNode->doubleConnector2->singleConnector
                               = neighborTransitionNode->doubleConnector2;
                else if( transitionNode == transitionNode->doubleConnector2->doubleConnector1 )
            		     transitionNode->doubleConnector2->doubleConnector1
                               = neighborTransitionNode->doubleConnector2;
                else if( transitionNode == transitionNode->doubleConnector2->doubleConnector2 )
            		     transitionNode->doubleConnector2->doubleConnector2
                               = neighborTransitionNode->doubleConnector2;                                
            }
            
            if( neighborTransitionNode->doubleConnector1->type == 'b' )   
            {
                if( neighborTransitionNode == neighborTransitionNode->doubleConnector1->south )
                	neighborTransitionNode->doubleConnector1->south 
                                       = transitionNode->doubleConnector1;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector1->north )
                		 neighborTransitionNode->doubleConnector1->north 
                                       = transitionNode->doubleConnector1;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector1->east )
                		 neighborTransitionNode->doubleConnector1->east 
                                       = transitionNode->doubleConnector1;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector1->west )
                    	 neighborTransitionNode->doubleConnector1->west 
                                       = transitionNode->doubleConnector1;
            }
            else if( neighborTransitionNode->doubleConnector1->type == 'w' )
            {
                if( neighborTransitionNode == neighborTransitionNode->doubleConnector1->singleConnector )
                	neighborTransitionNode->doubleConnector1->singleConnector
                                       = transitionNode->doubleConnector1;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector1->doubleConnector1 )
                		 neighborTransitionNode->doubleConnector1->doubleConnector1
                                       = transitionNode->doubleConnector1;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector1->doubleConnector2 )
                		 neighborTransitionNode->doubleConnector1->doubleConnector2
                                       = transitionNode->doubleConnector1;
            }  
              
            if( neighborTransitionNode->doubleConnector2->type == 'b' )   
            {
                if( neighborTransitionNode == neighborTransitionNode->doubleConnector2->south )
                	neighborTransitionNode->doubleConnector2->south 
                                       = transitionNode->doubleConnector2;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector2->north )
                		 neighborTransitionNode->doubleConnector2->north 
                                       = transitionNode->doubleConnector2;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector2->east )
                		 neighborTransitionNode->doubleConnector2->east 
                                       = transitionNode->doubleConnector2;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector2->west )
                		 neighborTransitionNode->doubleConnector2->west 
                                       = transitionNode->doubleConnector2;
            }
            else if( neighborTransitionNode->doubleConnector2->type == 'w' )
            {
                if( neighborTransitionNode == neighborTransitionNode->doubleConnector2->singleConnector )
                	neighborTransitionNode->doubleConnector2->singleConnector
                                       = transitionNode->doubleConnector2;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector2->doubleConnector1 )
                		 neighborTransitionNode->doubleConnector2->doubleConnector1
                                       = transitionNode->doubleConnector2;
                else if( neighborTransitionNode == neighborTransitionNode->doubleConnector2->doubleConnector2 )
                		 neighborTransitionNode->doubleConnector2->doubleConnector2
                                       = transitionNode->doubleConnector2;
            }     

            delete transitionNode;
            delete neighborTransitionNode;
        }
    }

}//end simplify() function

/*______________________________________________________________________________
________________________________________________________________________________
                           
						   FUNCTION drawHilbertCurveLocally()

    Redraws Hilbert's curve on the cell to be refined and on the previous and
	next cells.
	
________________________________________________________________________________
______________________________________________________________________________*/
void drawHilbertCurveLocally( Cell *gridCell, int colorValue, bool refined )
{
    setColor( colorValue );
    if( !refined )
    {
        if( gridCell->previous != 0 )
        drawLine( gridCell->previous->centerX, gridCell->previous->centerY,
                  gridCell->centerX, gridCell->centerY );
        if( gridCell->next != 0 )
        drawLine( gridCell->centerX, gridCell->centerY,
                  gridCell->next->centerX, gridCell->next->centerY );
    }
    else 
    {
        if( gridCell->previous != 0 )
        drawLine( gridCell->previous->centerX, gridCell->previous->centerY,
                  gridCell->centerX, gridCell->centerY );
                  
        drawLine( gridCell->centerX, gridCell->centerY,
                  gridCell->next->centerX, gridCell->next->centerY );                  
        
        drawLine( gridCell->next->centerX, gridCell->next->centerY,
                  gridCell->next->next->centerX, gridCell->next->next->centerY );                
        
        drawLine( gridCell->next->next->centerX, gridCell->next->next->centerY,
                  gridCell->next->next->next->centerX, gridCell->next->next->next->centerY );
        
        if( gridCell->next->next->next->next != 0 )
        drawLine( gridCell->next->next->next->centerX, 
                  gridCell->next->next->next->centerY,
                  gridCell->next->next->next->next->centerX, 
                  gridCell->next->next->next->next->centerY );
    }   
}//end drawHilbertCurveLocally() function

//end file refinement.cpp
