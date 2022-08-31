#pragma once
#include "CellNode.h"

namespace alg {
	/*
	Auxilary class used when refining a cell or creating a new grid.
	This class encapsulates the creation of 4 cells connected to eachother:
	NorthEast, NorthWest, SouthEast and SouthWest.

	WARNING: This class DOES NOT FREE THE MEMORY ALLOCATED FOR EACH CELL
	*/
	class CellBunch {
	public:
		void Init(Vector2D center, uint32_t refinement_level);

	public:
		CellNode *NE;
		CellNode *NW;
		CellNode *SW;
		CellNode *SE;
	};
}

