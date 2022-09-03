#pragma once
#include "CellNode.h"

namespace alg {
	/*
	Auxilary struct used when refining a cell or creating a new grid.
	This class encapsulates the creation of 4 cells connected to eachother:
	NorthEast, NorthWest, SouthEast and SouthWest.

	WARNING: This class DOES NOT FREE THE MEMORY ALLOCATED FOR EACH CELL
	*/
	struct CellBunch {

		CellBunch() = default;
		CellBunch(Vector2D center, uint32_t refinement_level);

		CellNode* NE;
		CellNode* NW;
		CellNode* SW;
		CellNode* SE;

		CellNode* first;
		CellNode* last;
	};
}

