#include "grid.h"
#include "../../System/graphics.h"

#include <string>
#include <iostream>

using std::cout;
using std::endl;

/*==============================================================================
                            FUNCTION drawGrid()

    Draws the whole grid.
==============================================================================*/
void Grid::drawGrid( int color ) {
	if( color < 0 ) {
		cout <<
        "\nWarning Grid::drawGrid(): Parameter color can not be negative numbers."
  		<< endl;
	}

	CellNode *gridCell = firstCell;
	setColor( color );
	disableLighting();
	setLineWidth ( 1.0 );
	for( int i = 0; i < numberOfCells; i++ ){
		if( gridCell->active  && gridCell->isToPaint )
			gridCell->drawWiredCell();

		gridCell = gridCell->next;
	}
}

/*==============================================================================
                             FUNCTION drawDomain()
==============================================================================*/
void Grid::drawDomain( int color ) {
	if( color < 0 ) {
		cout <<
        "\nWarning Grid::drawGrid(): Parameter color can not be negative numbers."
   		<< endl;
	}

    enableLighting();
	CellNode *gridCell = firstCell;
	setColor( color );
	while( gridCell != 0 ) {
		if( gridCell->active && gridCell->isToPaint )
			gridCell->drawSolidCell();

		gridCell = gridCell->next;
	}
}

/*==============================================================================
                           FUNCTION drawHilbertCurve()

    Draws Hilbert's curve in whole grid.
==============================================================================*/
void Grid::drawHilbertCurve( int color ) {
	if( color < 0 ) {
		cout <<
        "\nWarning Grid::drawGrid(): Parameter color can not be negative numbers."
   		<< endl;
	}

	disableLighting();
	CellNode *gridCell = firstCell;
	setColor( color );
	setLineWidth( 2 );
	while( gridCell->next != 0 ) {
		drawLine( gridCell->centerX,
				  gridCell->centerY,
				  gridCell->centerZ,
				  gridCell->next->centerX,
				  gridCell->next->centerY,
				  gridCell->next->centerZ
				);

		gridCell = gridCell->next;
	}
}

/*==============================================================================
                       FUNCTION drawCellsNumber()

    Writes the number of each cell of a grid according to the ordering given  by
    Hilbert's curve.
==============================================================================*/
void Grid::drawCellsNumber( int color, void* font ) {
	if( color < 0 ) {
		cout <<
        "\nWarning Grid::drawGrid(): Parameter color can not be negative numbers."
   		<< endl;
	}

	disableLighting();
	CellNode *gridCell = firstCell;
	char *cellNumber = (char *)malloc(20*sizeof(char));
	setColor( color );
	while( gridCell != 0 ) {
		sprintf( cellNumber, "%ld", gridCell->gridPosition );
		drawString( cellNumber,
					font,
					gridCell->centerX,
					gridCell->centerY,
					gridCell->centerZ + 0.017
				  );

		gridCell = gridCell->next;
	}
}

/*==============================================================================
                            FUNCTION print()

    Prints information about all cells of the grid in an output file named
    outputCell.txt. This file is  created  in  the  same  directory  where
    the the executable file of this program has been placed.
==============================================================================*/
void Grid::print() {
	CellNode *gridCell = firstCell;
	while( gridCell != 0 ) {
		gridCell->print();
		gridCell = gridCell->next;
	}
}

/*==============================================================================
                         FUNCTION drawALGgraph()

    Draws the correspondent graph of the ALG-3D data structure. Its parameters
    are: cell nodes color, transition nodes color, cell nodes links  color,  and
	transition nodes links color.
==============================================================================*/
void Grid::drawGraph( int CNColor, int TNColor, int CNLinkColor, int TNLinkColor ) {
	if( (CNColor < 0) || (TNColor < 0) || (CNLinkColor < 0) || (TNLinkColor < 0) ) {
		cout <<
		"\nWarning Grid::drawGrid(): Parameters of colors can not be negative numbers."
  		<< endl;
	}

	CellNode *gridCell;
	TransitionNode *whiteCell,
	          *transitionNode;

	Cell *neighbourCell[6],
		 *quadrupleConnector[6],
	     *headCell,
	     *tailCell;

	int pointSize = 7;
	bool continueDrawing;
	setLineWidth ( 1.0 );

	/*------------------- Painting links between black nodes. ----------------*/
	setColor( CNLinkColor );
	disableLighting();
	gridCell = firstCell;
	while( gridCell != 0 ) {
		neighbourCell[0] = gridCell->south;
		neighbourCell[1] = gridCell->west;
		neighbourCell[2] = gridCell->back;
		for( int i = 0; i < 3; i++ ){
			if( neighbourCell[i]->type != 'w' ){
				drawLine( gridCell->centerX,
						  gridCell->centerY,
						  gridCell->centerZ,
						  neighbourCell[i]->centerX,
						  neighbourCell[i]->centerY,
						  neighbourCell[i]->centerZ );
			}
		}
		gridCell = gridCell->next;
	}

	/*---------- Painting links between white nodes and black nodes. ---------*/
	setColor( TNLinkColor );
	gridCell = firstCell;
	while( gridCell != 0 ){
		neighbourCell[0] = gridCell->north;
		neighbourCell[1] = gridCell->south;
		neighbourCell[2] = gridCell->east;
		neighbourCell[3] = gridCell->west;
		neighbourCell[4] = gridCell->front;
		neighbourCell[5] = gridCell->back;
		for( int i = 0; i < 6; i++ ) {
			if( neighbourCell[i]->type == 'w' ) {
				whiteCell = dynamic_cast<TransitionNode*>(neighbourCell[i]);

				if( whiteCell->singleConnector == gridCell ){
					drawLine( whiteCell->centerX,
						      whiteCell->centerY,
						      whiteCell->centerZ,
						      gridCell->centerX,
						      gridCell->centerY,
						      gridCell->centerZ
							);
				}
				else if( whiteCell->quadrupleConnector1 == gridCell ){
					tailCell       = gridCell;
					headCell       = whiteCell;
					transitionNode = whiteCell;
					continueDrawing = true;

					while( (headCell != 0) && (headCell->type == 'w') && (continueDrawing) ){

						quadrupleConnector[0] = transitionNode->quadrupleConnector1;
						quadrupleConnector[1] = transitionNode->quadrupleConnector2;
						quadrupleConnector[2] = transitionNode->quadrupleConnector3;
						quadrupleConnector[3] = transitionNode->quadrupleConnector4;
						for( int i = 0; i < 4; i++ ){
							drawLine( transitionNode->centerX,
									  transitionNode->centerY,
									  transitionNode->centerZ,
									  quadrupleConnector[i]->centerX,
									  quadrupleConnector[i]->centerY,
									  quadrupleConnector[i]->centerZ );
						}

						tailCell = headCell;
						headCell = transitionNode->singleConnector;
						transitionNode = dynamic_cast<TransitionNode*> (headCell);
						if( (transitionNode != 0) && (transitionNode->quadrupleConnector1 != tailCell) ) {
							continueDrawing = false;
						}
					}
				}
			}
		}
		gridCell = gridCell->next;
	}

	/*------------------------- Painting black nodes -------------------------*/
	enableLighting();
	setColor( CNColor );
	gridCell = firstCell;
	while( gridCell != 0 ){
		gridCell->draw( pointSize );
		gridCell = gridCell->next;
	}

	/*------------------------- Painting white nodes -------------------------*/
	setColor( TNColor );
	gridCell = firstCell;
	while( gridCell != 0 ){
		neighbourCell[0] = gridCell->north;
		neighbourCell[1] = gridCell->south;
		neighbourCell[2] = gridCell->east;
		neighbourCell[3] = gridCell->west;
		neighbourCell[4] = gridCell->front;
		neighbourCell[5] = gridCell->back;
		for( int i = 0; i < 6; i++ ) {
			if( neighbourCell[i]->type == 'w' ) {
				whiteCell = dynamic_cast<TransitionNode*>(neighbourCell[i]);
				if( whiteCell->quadrupleConnector1 == gridCell ){
					tailCell        = gridCell;
					headCell        = whiteCell;
					transitionNode  = whiteCell;
					continueDrawing = true;

					while( (headCell != 0) && (headCell->type == 'w') && (continueDrawing) ){
						transitionNode->draw( pointSize );

						tailCell = headCell;
						headCell = transitionNode->singleConnector;
						transitionNode = dynamic_cast<TransitionNode*> (headCell);
						if( (transitionNode != 0) && (transitionNode->quadrupleConnector1 != tailCell) ) {
							continueDrawing = false;
						}
					}
				}
			}
		}
		gridCell = gridCell->next;
	}
}

/*==============================================================================
                       FUNCTION drawTransitionNodesDirection()

    Writes the direction of each transition node on screen. Its parameter are
	the color number and a pointer to the font desired for texts.
==============================================================================*/
void Grid::drawTransitionNodesDirection( int color, void *font ) {
	if( color < 0 ) {
		cout <<
        "\nWarning Grid::drawGrid(): Parameter color can not be negative numbers."
  		<< endl;
	}

	CellNode *gridCell;

	TransitionNode *whiteCell,
	          *transitionNode;

	Cell *neighbourCell[6],
	     *headCell,
	     *tailCell;

	bool continueDrawing;
	char *direction = (char *)malloc(20*sizeof(char));

	setColor( color );
	disableLighting();
	gridCell = firstCell;
	while( gridCell != 0 ){
		neighbourCell[0] = gridCell->north;
		neighbourCell[1] = gridCell->south;
		neighbourCell[2] = gridCell->east;
		neighbourCell[3] = gridCell->west;
		neighbourCell[4] = gridCell->front;
		neighbourCell[5] = gridCell->back;
		for( int i = 0; i < 6; i++ ) {
			if( neighbourCell[i]->type == 'w' ) {
				whiteCell = dynamic_cast<TransitionNode*>(neighbourCell[i]);

				if( whiteCell->singleConnector == gridCell ){
					drawLine( whiteCell->centerX,
						      whiteCell->centerY,
						      whiteCell->centerZ,
						      gridCell->centerX,
						      gridCell->centerY,
						      gridCell->centerZ
							);
				}
				else if( whiteCell->quadrupleConnector1 == gridCell ){
					tailCell       = gridCell;
					headCell       = whiteCell;
					transitionNode = whiteCell;
					continueDrawing = true;
					while( (headCell != 0) && (headCell->type == 'w') && (continueDrawing) ){
						switch( transitionNode->direction ) {
						    case 'n':
								 sprintf( direction, "%s", "North" );
								 break;
			 				case 's':
								 sprintf( direction, "%s", "South" );
								 break;
				 			case 'e':
								 sprintf( direction, "%s", "East" );
								 break;
							case 'w':
								 sprintf( direction, "%s", "West" );
								 break;
							case 'f':
								 sprintf( direction, "%s", "Front" );
								 break;
							case 'b':
								 sprintf( direction, "%s", "Back" );
								 break;
							default:
								sprintf( direction, "%s", "unknown" );
								break;
						}
						drawString( direction,
									font,
									transitionNode->centerX,
									transitionNode->centerY,
									transitionNode->centerZ + 0.017);

						tailCell = headCell;
						headCell = transitionNode->singleConnector;
						transitionNode = dynamic_cast<TransitionNode*> (headCell);
						if( (transitionNode != 0) && (transitionNode->quadrupleConnector1 != tailCell) ) {
							continueDrawing = false;
						}
					}
				}
			}
		}
		gridCell = gridCell->next;
	}
}

void Grid::setTopPlane( float top ) {
    CellNode *gridCell = firstCell;
	while( gridCell != 0 ) {
        if( gridCell->centerZ > top ) gridCell->isToPaint = false;
        else gridCell->isToPaint = true;

		gridCell = gridCell->next;
	}
}
