/**
 * @file cell_draw.cpp
 * File where all methods of class CellNode which are related to drawings of
 * cells and printing of information about them are implemented.
 */

#include <iostream>
#include <fstream>
#include <string>

#include "cell.h"

using std::ofstream;
using std::ios;
using std::endl;

/**
 * Prints  relevant  informations about  cell  nodes on file named outputCellNode.txt.
 * If this file does not exist, a new file will be created in the same directory where
 * this program was invoked.  If  it  already  exist, the  file  will  be  opened  and
 * all information will be appended at the end of it.
 */
void CellNode::print() {
	ofstream file("outputCellNode.txt", ios::app);
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

/**
 * Prints relevant informations about transition  nodes on  a file named
 * outputTransitionNode.txt. If this file does  not  exist, a  new  file
 * will be created in the same directory where this program was invoked.
 * If it already exist, the file will be opened and all information will
 * be appended at the end of it.
 */
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
