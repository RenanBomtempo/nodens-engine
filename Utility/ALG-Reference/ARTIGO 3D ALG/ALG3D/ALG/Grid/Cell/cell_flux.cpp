#include "cell.h"
#include <cmath>


/*==============================================================================
                          FUNCTION setCellFlux()

    Computes the flux into this cell due to a neighbor cell.

    flux = (u(thisCell) - u(neighborCell) / 2* leastDistance,

    where 2* leastDistance is two times the least of the lengths of the faces of
    this cell and its neighbor.
==============================================================================*/
void CellNode::setCellFlux( char direction ) throw (InvalidParameterValue){

	 Cell *neighborGridCell;
	 TransitionNode *whiteNeighborCell;
	 CellNode *blackNeighborCell;

	 switch( direction ) {
 		 case 'n':
		 	  neighborGridCell = north;
		 	  break;

 		 case 's':
		 	  neighborGridCell = south;
		 	  break;

 		 case 'e':
		 	  neighborGridCell = east;
		 	  break;

 		 case 'w':
		 	  neighborGridCell = west;
		 	  break;

 		 case 'f':
		 	  neighborGridCell = front;
		 	  break;

 		 case 'b':
		 	  neighborGridCell = back;
		 	  break;

		default:
			  throw InvalidParameterValue
			  ("CellNode::setCellFlux(): Parameter direction has invalid value.", direction);
	 }

	float leastDistance = halfFaceLength;
	float localFlux;
	bool hasFound;

	// difference between levels of this cell and its neighbor cell.
	int levelDifference;

	/* When neighborGridCell is a transition node, looks for the next neighbor
	 * cell which is a cell node. */
	if( (neighborGridCell->level <= level) && (neighborGridCell->type == 'w') ) {
		whiteNeighborCell = static_cast<TransitionNode*>(neighborGridCell);
		hasFound = false;
	   	while( !hasFound ) {
		   if( neighborGridCell->type == 'w' ) {
		   	   whiteNeighborCell = dynamic_cast<TransitionNode*>(neighborGridCell);
		   	   if( whiteNeighborCell->singleConnector == 0 ) {
			   	   hasFound = true;
			   }
			   else {
			   		neighborGridCell = whiteNeighborCell->singleConnector;
	   		   }
		   }
		   else {
		   		break;
   		   }
		}
	}

	/* It enters this "if" only if neighborGridCell is connected to null  or  if
	 * it is not active, i.e., only if gridCell belongs to the boundary  of  the
	 * domain. */
	if( (neighborGridCell->type == 'w') || (!neighborGridCell->active) ) {
		localFlux = u / halfFaceLength;
		switch( direction ) {
			case 's':
				if ( localFlux > southFlux ) {
					southFlux = localFlux;
				}
				break;

			case 'n':
				if ( localFlux > northFlux ) {
					northFlux = localFlux;
				}
				break;

			case 'e':
				if ( localFlux > eastFlux ) {
					eastFlux = localFlux;
				}
				break;

			case 'w':
				if ( localFlux > westFlux ) {
					westFlux = localFlux;
				}
				break;

			case 'f':
				if ( localFlux > frontFlux ) {
					frontFlux = localFlux;
				}
				break;

			case 'b':
				if ( localFlux > backFlux ) {
					backFlux = localFlux;
				}
				break;
		}
	}
	//Enters here almost always, except in the case indicated above.
	else if( ( neighborGridCell->type == 'b' ) && ( neighborGridCell->active ) ) {
		 blackNeighborCell = static_cast<CellNode*>(neighborGridCell);
		/* The following piece of code avoids double computations. That  is,  if
		 * the program has already computed the total flux for a pair  of  cells
		 * (gridCell and its neighbor blackNeighborCell), it will not  pass  here
		 * again when their role is  reversed  (i.e.,  blackNeighborCell  becomes
		 * gridCell). Moreover, it also guarantees that no pair of cells is left
		 * without its flux being computed. */
		 if( (level >= blackNeighborCell->level) &&
		     ( (centerX >= blackNeighborCell->centerX) &&
			   (centerY >= blackNeighborCell->centerY) &&
			   (centerZ > blackNeighborCell->centerZ)
			 )
		   )
		{
			levelDifference = level - blackNeighborCell->level;

			if ( blackNeighborCell->halfFaceLength < leastDistance )
				leastDistance = blackNeighborCell->halfFaceLength;

			localFlux = ( u - blackNeighborCell->u ) / ( 2 * leastDistance );
			switch( direction ) {
				case 's':
					if ( localFlux > southFlux )
						southFlux += localFlux;
					if ( localFlux > blackNeighborCell->northFlux )
						blackNeighborCell->northFlux += localFlux;
					break;

				case 'n':
					if ( localFlux > northFlux )
						northFlux += localFlux;
					if ( localFlux > blackNeighborCell->southFlux )
						blackNeighborCell->southFlux += localFlux;
					break;

				case 'e':
					if ( localFlux > eastFlux )
						eastFlux += localFlux;
					if ( localFlux > blackNeighborCell->westFlux )
						blackNeighborCell->westFlux += localFlux;
					break;

				case 'w':
					if ( localFlux > westFlux )
						westFlux += localFlux;
					if ( localFlux > blackNeighborCell->eastFlux )
						blackNeighborCell->eastFlux += localFlux;
					break;

				case 'f':
					if ( localFlux > frontFlux )
						frontFlux += localFlux;
					if ( localFlux > blackNeighborCell->backFlux )
						blackNeighborCell->backFlux += localFlux;

				case 'b':
					if ( localFlux > backFlux )
						backFlux += localFlux;
					if ( localFlux > blackNeighborCell->frontFlux )
						blackNeighborCell->frontFlux += localFlux;
					break;
			}
	    }
	}
}

/*==============================================================================

                          FUNCTION getMaximumCellFlux()

   Returns the highest flux coming from the six sides of a cell.
==============================================================================*/
float CellNode::getMaximumFlux() {

	float maximumFlux = fabs(eastFlux);
	if( fabs(northFlux) > maximumFlux )
		maximumFlux = fabs(northFlux);

	if( fabs(southFlux) > maximumFlux )
		maximumFlux = fabs(southFlux);

	if( fabs(westFlux) > maximumFlux )
		maximumFlux = fabs(westFlux);

	if( fabs(frontFlux) > maximumFlux )
		maximumFlux = fabs(frontFlux);

	if( fabs(backFlux) > maximumFlux )
		maximumFlux = fabs(backFlux);

	return maximumFlux;
}
