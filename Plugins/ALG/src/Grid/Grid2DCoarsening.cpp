#include "algpch.h"
#include "MHC/MHC.h"
#include "Grid/Grid2D.h"

namespace alg {
	void Grid2D::CoarsenGrid()
	{
		auto cell = m_FirstCell;
		while (cell != nullptr)
		{
			auto next = cell->m_Next;
			cell = CoarsenBunch(cell->GetBunch());
		}

	}
	
	CellNode* Grid2D::CoarsenBunch(CellBunch& old_bunch)
	{
		CellNode* new_cell = new CellNode();
		
		// Calculate new cell's center
		new_cell->m_Center.x = (old_bunch.NW->m_Center.x + old_bunch.NE->m_Center.x) / 2;
		new_cell->m_Center.y = (old_bunch.NW->m_Center.y + old_bunch.SW->m_Center.y) / 2;
		
		
		//______________________________________________________________________
		// Reconnect North
		switch (old_bunch.NE->m_North->GetType())
		{
		case NodeType::Cell:
			_CoarsenCase2(
				(CellNode*)old_bunch.NE->m_North,
				(CellNode*)old_bunch.NW->m_North, new_cell, Direction::North);
			break;
		case NodeType::Transition:
			_CoarsenCase1(
				(CellNode*)old_bunch.NE->m_North, new_cell, Direction::North);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		//______________________________________________________________________
		// Connect South
		switch (old_bunch.SE->m_South->GetType())
		{
		case NodeType::Cell:
			_CoarsenCase2(
				(CellNode*)old_bunch.SE->m_South,
				(CellNode*)old_bunch.SW->m_South,
				new_cell, Direction::South);
			break;
		case NodeType::Transition:
			//CoarsenCase1(cell_in_bunch, new_bunch, (TransitionNode*)(cell_in_bunch->m_South), Direction::South);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		//______________________________________________________________________
		// Connect West
		switch (old_bunch.SW->m_West->GetType())
		{
		case NodeType::Cell:
			_CoarsenCase2(
				(CellNode*)old_bunch.SW->m_West,
				(CellNode*)old_bunch.NW->m_West,
				new_cell, Direction::West);
			break;
		case NodeType::Transition:
			//CoarsenCase1(cell_in_bunch, new_bunch, (TransitionNode*)(cell_in_bunch->m_West), Direction::West);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		//______________________________________________________________________
		// Connect East
		switch (old_bunch.NE->m_East->GetType())
		{
		case NodeType::Cell:
			_CoarsenCase2(
				(CellNode*)old_bunch.SE->m_East,
				(CellNode*)old_bunch.NE->m_East,
				new_cell, Direction::East);
			break;
		case NodeType::Transition:
			//CoarsenCase1(cell_in_bunch, new_bunch, (TransitionNode*)(cell_in_bunch->m_East), Direction::East);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		// MHC Ordering
		_UpdateMHCAfterCoarsening(new_cell, old_bunch.first, old_bunch.last);

		return new_cell;
	}


	/* Old bunch neighbour is a cell.
	*         ___ .___          ___ .___
	*        /       /         /       /
	*       /_______/         /_______/
	*         /                  /
	*        T        =>        /
	*    ___/_\__          ____/___
	*   /___/___/         /       /
	*  /___/___/         /_______/
	*
	*/
	void Grid2D::_CoarsenCase1(CellNode* outer_cell, CellNode* new_cell, Grid2D::Direction direction)
	{
	}
	
	/* Old bunch's neighbour is also a bunch of the same refinement level.
	*  This means that the cells were directly connected.
	*         ____.___          ____.___
	*        /___/___/         /___/___/
	*       /___/___/         /___/___/
	*        /   /              \ /
	*       /   /    =>          T
	*    __/___/_          ___ /___
	*   /___/___/         /       /
	*  /___/___/         /_______/
	*
	*/
	void Grid2D::_CoarsenCase2(CellNode* outer_cell_SorE, CellNode* outer_cell_NorW, CellNode* new_cell, Grid2D::Direction direction)
	{
		// Create Transition node
		TransitionNode* new_transition = new TransitionNode();
		new_transition->m_Lower = new_cell;
		new_transition->m_HigherNorW = outer_cell_NorW;
		new_transition->m_HigherSorE = outer_cell_SorE;

		switch (direction)
		{
		case Grid2D::Direction::North:
			outer_cell_NorW->m_South = new_transition;
			outer_cell_SorE->m_South = new_transition;
			new_cell->m_North = new_transition;
			break;
		case Grid2D::Direction::South:
			outer_cell_NorW->m_North = new_transition;
			outer_cell_SorE->m_North = new_transition;
			new_cell->m_South = new_transition;
			break;
		case Grid2D::Direction::West:
			outer_cell_NorW->m_East = new_transition;
			outer_cell_SorE->m_East = new_transition;
			new_cell->m_West = new_transition;
			break;
		case Grid2D::Direction::East:
			outer_cell_NorW->m_West = new_transition;
			outer_cell_SorE->m_West = new_transition;
			new_cell->m_East = new_transition;
			break;
		default:
			break;
		}
	}

	void Grid2D::_CoarsenCase2a(CellBunch& bunch, CellNode* new_cell, Grid2D::Direction direction)
	{
	}

	void Grid2D::_CoarsenCase2b(CellBunch& bunch, CellNode* new_cell, Grid2D::Direction direction)
	{
	}

	void Grid2D::_UpdateMHCAfterCoarsening(CellNode* new_cell, CellNode* old_first_cell, CellNode* old_last_cell)
	{

		new_cell->m_GlobalIndex = old_first_cell->BunchIndex();
		new_cell->m_RefinementLevel = old_first_cell->m_RefinementLevel - 1;
		new_cell->m_Previous = old_first_cell->m_Previous;
		new_cell->m_Next = old_last_cell->m_Next;

		if (old_first_cell == FirstCell())
			m_FirstCell = new_cell;
		if (old_last_cell == LastCell())
			m_LastCell = new_cell;
		
		m_NumberOfCells -= 3;
	}
}