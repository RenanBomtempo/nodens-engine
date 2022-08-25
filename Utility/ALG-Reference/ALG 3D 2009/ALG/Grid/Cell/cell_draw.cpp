#include <iostream>
#include <fstream>
#include <string>

#include "cell.h"
#include "../../../System/graphics.h"

using std::ofstream;
using std::ios;
using std::endl;

/*==============================================================================
                              FUNCTION draw()																				

    Draws a graph node as sphere.
==============================================================================*/
void Cell::draw( int size ) {
	 drawSphere( centerX, centerY, centerZ, size );
}

/*==============================================================================
                         FUNCTION drawWiredCell()																				

    Draws a grid cell as a wired cube.
==============================================================================*/
void CellNode::drawWiredCell() {
 	float x0, y0, z0, x1, y1, z1;
	x0 = centerX - halfFaceLength;
	y0 = centerY - halfFaceLength;
	z0 = centerZ - halfFaceLength;
	x1 = centerX + halfFaceLength;
	y1 = centerY + halfFaceLength;
	z1 = centerZ + halfFaceLength;
	drawCube( x0, y0, z0, x1, y1, z1 );
}

/*==============================================================================
                         FUNCTION drawSolidCell()																				

    Draws a grid cell as a solid cube.
==============================================================================*/
void CellNode::drawSolidCell() {
	float x0, y0, z0, x1, y1, z1;
	x0 = centerX - halfFaceLength;
	y0 = centerY - halfFaceLength;
	z0 = centerZ - halfFaceLength;
	x1 = centerX + halfFaceLength;
	y1 = centerY + halfFaceLength;
	z1 = centerZ + halfFaceLength;
	drawSolidCube( x0, y0, z0, x1, y1, z1 );
}

/*==============================================================================
                         FUNCTION CellNode::print()																				

    Prints specific information about transition nodes in an output file.
==============================================================================*/
void CellNode::print() {
	ofstream file("outputCell.txt", ios::app);
	file << "Cell " << gridPosition << "\n\n";
	file << "Type = " << type << "\n";
	file << "Active = " << active << "\n";
	file << "Level = " << level << "\n";
	file << "bunchNumber = " << bunchNumber << "\n";
	file << "HilberShapeNumber = " << hilbertShapeNumber << "\n\n";
	
	Cell *cell[] = {north, south, east, west, front, back};
	const char *cellName[] = {"north", "south", "east", "west", "front", "back"};
	TransitionNode *transitionNode;
	Cell *quadrupleConnector[4];
	
	for( int i = 0; i < 6; i++ ) {
		if( cell[i]->type == 'b' ) {
			file << "\t" << cellName[i] << " neighbor is black, has grid position "
			     << dynamic_cast<CellNode*>(cell[i])->gridPosition
				 << " and level " << cell[i]->level << ".\n";
		}
		else {
			 file << "\t" << cellName[i] << " neighbor is white and has level "
				  << cell[i]->level << ":\n";
			
			 transitionNode        = dynamic_cast<TransitionNode*>(cell[i]);
			 quadrupleConnector[0] = transitionNode->quadrupleConnector1;
			 quadrupleConnector[1] = transitionNode->quadrupleConnector2;
			 quadrupleConnector[2] = transitionNode->quadrupleConnector3;
			 quadrupleConnector[3] = transitionNode->quadrupleConnector4;
			 
			 if( transitionNode->singleConnector != 0 ) {
				  	  if( transitionNode->singleConnector->type == 'b' ) {
					  	  file << "\t\tsingleConnector = "
							   << dynamic_cast<CellNode*>(transitionNode->singleConnector)->gridPosition
						  	   << ".\n";
					  }
					  else {
					  	   file << "\t\tsingleConnector is white.\n";
	  	   			  }
			 }	
			 for( int j = 0; j < 4; j++ ) {
			 	  if( quadrupleConnector[j] != 0 ) {
				  	  if( quadrupleConnector[j]->type == 'b' ) {
					  	  file << "\t\tquadrupleConnector" << (j + 1) << " = "
							   << dynamic_cast<CellNode*>(quadrupleConnector[j])->gridPosition
						  	   << ".\n";
					  }
					  else {
					  	   file << "\tquadrupleConnector" << (j + 1) << " is white.\n";
	  	   			  }
				  }
			 }
	 	}
 	}
				
	if( previous != 0 ) {
		file << "\nPrevious cell = " << previous->gridPosition << ".\n";
	}
	if( next != 0 ) {
		file << "Next cell = " << next->gridPosition << ".\n";
	}
	file << "Center = ( " << centerX << ", " << centerY << " ).\n";
	file << "Face length = " << faceLength << ".\n";
	file << "Half face length " << halfFaceLength << ".\n";
	file <<
	"________________________________________________________________________\n"
	<< endl;
}

/*==============================================================================
                          FUNCTION TransitionNode::print()																				

    Prints specific information about transition nodes in an output file.
==============================================================================*/
void TransitionNode::print() {
	ofstream file( "outputTransitionNode.txt", ios::app );

	file << "Transition Node" << endl;
	file << "Type = " << type << endl;
	file << "Level = " << level << endl;
	
	if( singleConnector != 0 ) {
		file << "Single Connector: " << singleConnector->type << endl;
	}
	else{
		file << "Single Connector = 0" << endl;
	}
	
	Cell *quadrupleConnector[4];
	quadrupleConnector[0] = quadrupleConnector1;
	quadrupleConnector[1] = quadrupleConnector2;
	quadrupleConnector[2] = quadrupleConnector3;
	quadrupleConnector[3] = quadrupleConnector4;
	
	for( int i = 0; i < 4; i++ ) {
	    if( quadrupleConnector[i] != 0 ) {
		    file << "Quadruple Connector " << (i + 1) <<  ": "
			     << quadrupleConnector[i]->type << endl;
		}
		else {
			file << "Quadruple Connector " << (i + 1) << " = 0" << endl;
		}
 	}
	file <<
	"________________________________________________________________________\n"
	<< endl;
}

/*==============================================================================
                          FUNCTION TransitionNode::print()																				

    Prints specific variable information for black node in an output file.
==============================================================================*/
void CellNode::printVariable() {
	ofstream file( "outputCellVariable.txt", ios::app );
	file << "CellNode " << gridPosition << endl;
	file << "u = " << u << "\n";
	file << "previousU = " << previousU << endl << endl;
}
