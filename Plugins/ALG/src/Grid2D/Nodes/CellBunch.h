#pragma once
#include "Grid2D/Nodes/CellNode.h"
#include "Grid2D/Nodes/TransitionNode.h"
#include "Math/Vector2D.h"

namespace alg {
	/*
	Auxilary struct used when refining a cell or creating a new grid.
	This class encapsulates the creation of 4 cells connected to eachother:
	NorthEast, NorthWest, SouthEast and SouthWest.

	WARNING: This class DOES NOT FREE THE MEMORY ALLOCATED FOR EACH CELL
	*/

	struct CellBunch {
		friend class Grid2D;
		friend class CellNode;
		friend class TransitionNode;
	public:
		CellBunch();
		void Initialize(Vector2D center, uint8_t refinement_level);
		void Destroy();

		Vector2D CalculateCenter();

	private:
		void ConnectToAnotherBunch(Direction direction, CellBunch& bunch);
		void ConnectToTransition(Direction direction, TransitionNode* transition_node);

		CellNode* NE;
		CellNode* NW;
		CellNode* SW;
		CellNode* SE;

		CellNode* first;
		CellNode* last;
	};
}

