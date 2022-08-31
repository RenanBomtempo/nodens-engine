#include "algpch.h"
#include "Log.h"
#include "Grid2D.h"
#include "MHC/MHC.h"

namespace alg {
	/**
	* /brief Refine a cell and returns a reference to the first cell from
	* the newly created cell bunch.
	*
	* /param old_cell: The cell that must be refined.
	*
	* /returns A reference to the first cell in the new bunch.
	*/
	CellNode* const Grid2D::RefineCell(CellNode* old_cell)
	{
		ALG_CORE_ASSERT(m_NumberOfCells > 0, "Grid has not been initialized!");
		ALG_CORE_ASSERT(old_cell != nullptr, "RefineCell: 'old_cell' is nullptr.");

		ALG_CORE_INFO("Refining cell {0}", old_cell->GetMHCIndexAsBinaryString());

		if (old_cell->m_RefLevel == MAX_REFINEMENT_LEVEL)
		{
			std::cout << "Can't refine cell " << old_cell->m_MHCIndex << " . It has reached the maximum refinement level.";
			return nullptr;
		}
		// =====================================================================
		// Connect Neighbors

		uint8_t new_refinement_level = old_cell->m_RefLevel + 1;
		CellBunch new_bunch;
		new_bunch.Init(old_cell->m_Center, new_refinement_level);

		//______________________________________________________________________
		// Connect North
		switch (old_cell->m_North->GetType())
		{
		case NodeType::Cell:
			ConnectCase1(new_bunch, (CellNode*)(old_cell->m_North), Direction::North);
			break;
		case NodeType::Transition:
			ConnectCase2(old_cell, new_bunch, (TransitionNode*)(old_cell->m_North), Direction::North);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		//______________________________________________________________________
		// Connect South
		switch (old_cell->m_South->GetType())
		{
		case NodeType::Cell:
			ConnectCase1(new_bunch, (CellNode*)(old_cell->m_South), Direction::South);
			break;
		case NodeType::Transition:
			ConnectCase2(old_cell, new_bunch, (TransitionNode*)(old_cell->m_South), Direction::South);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		//______________________________________________________________________
		// Connect West
		switch (old_cell->m_West->GetType())
		{
		case NodeType::Cell:
			ConnectCase1(new_bunch, (CellNode*)(old_cell->m_West), Direction::West);
			break;
		case NodeType::Transition:
			ConnectCase2(old_cell, new_bunch, (TransitionNode*)(old_cell->m_West), Direction::West);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		//______________________________________________________________________
		// Connect East
		switch (old_cell->m_East->GetType())
		{
		case NodeType::Cell:
			ConnectCase1(new_bunch, (CellNode*)(old_cell->m_East), Direction::East);
			break;
		case NodeType::Transition:
			ConnectCase2(old_cell, new_bunch, (TransitionNode*)(old_cell->m_East), Direction::East);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		// =====================================================================
		// Update MHC Ordering
		UpdateMHC(old_cell, new_bunch);
		//CellNode* first_cell_in_new_bunch = old_cell->m_MHCPrevious->m_MHCNext;

		// =====================================================================
		// Process Data Payload
		// 
		// Call the user defined function responsible for processing the 
		// datapayload inside the newly created cells. 

		this->m_NumberOfCells += 3;

		// Isolate cell before deleting it
		old_cell->m_North = nullptr;
		old_cell->m_East = nullptr;
		old_cell->m_South = nullptr;
		old_cell->m_West = nullptr;
		delete old_cell;

		return nullptr;
	}

	void Grid2D::ConnectCase1(CellBunch& new_bunch, CellNode* external_cell, Direction direction)
	{
		// Before refining neighbouring cells HAD THE SAME LEVEL of refinement.

		TransitionNode* new_transition = new TransitionNode();
		new_transition->m_Lower = external_cell;

		switch (direction)
		{
		case Direction::North:
			external_cell->m_South = new_transition;
			new_bunch.NW->m_North = new_transition;
			new_bunch.NE->m_North = new_transition;
			new_transition->m_HigherNorW = new_bunch.NW;
			new_transition->m_HigherSorE = new_bunch.NE;
			break;
		case Direction::South:
			external_cell->m_North = new_transition;
			new_bunch.SW->m_South = new_transition;
			new_bunch.SE->m_South = new_transition;
			new_transition->m_HigherNorW = new_bunch.SW;
			new_transition->m_HigherSorE = new_bunch.SE;
			break;
		case Direction::West:
			external_cell->m_East = new_transition;
			new_bunch.NW->m_West = new_transition;
			new_bunch.SW->m_West = new_transition;
			new_transition->m_HigherNorW = new_bunch.NW;
			new_transition->m_HigherSorE = new_bunch.SW;
			break;
		case Direction::East:
			external_cell->m_West = new_transition;
			new_bunch.NE->m_East = new_transition;
			new_bunch.SE->m_East = new_transition;
			new_transition->m_HigherNorW = new_bunch.NE;
			new_transition->m_HigherSorE = new_bunch.SE;
			break;
		default:
			break;
		}
	}


	void Grid2D::ConnectCase21(CellBunch& new_bunch, TransitionNode* transition, Direction direction)
	{
		// Connect HIGHER SOUTH/EAST
		if (NodeType::Cell == transition->m_HigherSorE->GetType())
		{
			// Case 2.1.1
			// Transition connects directly to another cell
			CellNode* external_cell = (CellNode*)transition->m_HigherSorE;

			switch (direction)
			{
				// EAST
			case Direction::North:
				new_bunch.NE->m_North = external_cell;
				external_cell->m_South = new_bunch.NE;
				break;
			case Direction::South:
				new_bunch.SE->m_South = external_cell;
				external_cell->m_North = new_bunch.SE;
				break;

				// SOUTH
			case Direction::West:
				new_bunch.SW->m_West = external_cell;
				external_cell->m_East = new_bunch.SW;
				break;
			case Direction::East:
				new_bunch.SE->m_East = external_cell;
				external_cell->m_West = new_bunch.SE;
				break;
			default:
				break;
			}
		}
		else if (NodeType::Transition == transition->m_HigherSorE->GetType())
		{
			// Case 2.1.2
			// Transition connects to another transition
			TransitionNode* external_transition = (TransitionNode*)transition->m_HigherSorE;

			switch (direction)
			{
				// Higher EAST
			case Direction::North:
				new_bunch.NE->m_North = external_transition;
				external_transition->m_Lower = new_bunch.NE;
				break;
			case Direction::South:
				new_bunch.SE->m_South = external_transition;
				external_transition->m_Lower = new_bunch.SE;
				break;

				// Higher SOUTH
			case Direction::West:
				new_bunch.SW->m_West = external_transition;
				external_transition->m_Lower = new_bunch.SW;
				break;
			case Direction::East:
				new_bunch.SE->m_East = external_transition;
				external_transition->m_Lower = new_bunch.SE;
				break;
			default:
				break;
			}
		}

		// Connect HIGHER NORTH/WEST
		if (NodeType::Cell == transition->m_HigherNorW->GetType())
		{
			// Case 2.1.1
			// Transition connects directly to another cell
			CellNode* external_cell = (CellNode*)transition->m_HigherNorW;

			switch (direction)
			{
			case Direction::North:
				new_bunch.NW->m_North = external_cell;
				external_cell->m_South = new_bunch.NW;
				break;
			case Direction::South:
				new_bunch.SW->m_South = external_cell;
				external_cell->m_North = new_bunch.SW;
				break;
			case Direction::West:
				new_bunch.NW->m_West = external_cell;
				external_cell->m_East = new_bunch.NW;
				break;
			case Direction::East:
				new_bunch.NE->m_East = external_cell;
				external_cell->m_West = new_bunch.NE;
				break;
			default:
				break;
			}
		}
		else if (NodeType::Transition == transition->m_HigherNorW->GetType())
		{
			// Case 2.1.2
			// Transition connects to another transition
			TransitionNode* external_transition = (TransitionNode*)transition->m_HigherNorW;

			switch (direction)
			{
				// Higher EAST
			case Direction::North:
				new_bunch.NW->m_North = external_transition;
				external_transition->m_Lower = new_bunch.NW;
				break;
			case Direction::South:
				new_bunch.SW->m_South = external_transition;
				external_transition->m_Lower = new_bunch.SW;
				break;

				// Higher SOUTH
			case Direction::West:
				new_bunch.NW->m_West = external_transition;
				external_transition->m_Lower = new_bunch.NW;
				break;
			case Direction::East:
				new_bunch.NE->m_East = external_transition;
				external_transition->m_Lower = new_bunch.NE;
				break;
			default:
				break;
			}
		}
	}

	void Grid2D::ConnectCase22(CellBunch& new_bunch, TransitionNode* new_transition, Direction direction)
	{
		switch (direction)
		{
		case Direction::North:
			new_bunch.NE->m_North = new_transition;
			new_bunch.NW->m_North = new_transition;
			new_transition->m_HigherSorE = new_bunch.NE;
			new_transition->m_HigherNorW = new_bunch.NW;
			break;
		case Direction::South:
			new_bunch.SE->m_South = new_transition;
			new_bunch.SW->m_South = new_transition;
			new_transition->m_HigherSorE = new_bunch.SE;
			new_transition->m_HigherNorW = new_bunch.SW;
			break;
		case Direction::West:
			new_bunch.NW->m_West = new_transition;
			new_bunch.SW->m_West = new_transition;
			new_transition->m_HigherNorW = new_bunch.NW;
			new_transition->m_HigherSorE = new_bunch.SW;
			break;
		case Direction::East:
			new_bunch.NE->m_East = new_transition;
			new_bunch.SE->m_East = new_transition;
			new_transition->m_HigherNorW = new_bunch.NE;
			new_transition->m_HigherSorE = new_bunch.SE;
			break;
		default:
			throw std::runtime_error("Invalid direction.");
			break;
		}
	}

	void Grid2D::UpdateMHC(CellNode* old_cell, CellBunch& new_bunch)
	{
		ALG_CORE_ASSERT(old_cell != nullptr, "BuildMHC: 'old_cell' is nullptr.");

		uint32_t mhc_index_offset = 2 * (old_cell->m_RefLevel - 1);

		switch (MHC::CalculateMHCProfile(old_cell->m_MHCIndex, old_cell->m_RefLevel))
		{
		case MHC::Profile::C:
			//   .____.     .____.
			//	 | NW | <== | NE |
			//   |____|	    |____|
			//     ||
			//     \/
			//   .____.     .____.
			//	 | SW | ==> | SE |
			//   |____|	    |____|
			// 
			// First cell
			new_bunch.NE->m_MHCIndex = old_cell->m_MHCIndex + (0b00 << mhc_index_offset);
			if (m_MHCFirstCell == old_cell)
			{
				new_bunch.NE->m_MHCPrevious = nullptr;
				m_MHCFirstCell = new_bunch.NE;
			}
			else {
				new_bunch.NE->m_MHCPrevious = old_cell->m_MHCPrevious;
				old_cell->m_MHCPrevious->m_MHCNext = new_bunch.NE;
			}
			new_bunch.NE->m_MHCNext = new_bunch.NW;

			// Second cell
			new_bunch.NW->m_MHCIndex = old_cell->m_MHCIndex + (0b01 << mhc_index_offset);
			new_bunch.NW->m_MHCPrevious = new_bunch.NE;
			new_bunch.NW->m_MHCNext = new_bunch.SW;

			// Third cell
			new_bunch.SW->m_MHCIndex = old_cell->m_MHCIndex + (0b10 << mhc_index_offset);
			new_bunch.SW->m_MHCPrevious = new_bunch.NW;
			new_bunch.SW->m_MHCNext = new_bunch.SE;

			// Last cell
			new_bunch.SE->m_MHCIndex = old_cell->m_MHCIndex + (0b11 << mhc_index_offset);
			new_bunch.SE->m_MHCPrevious = new_bunch.SW;
			if (m_MHCLastCell == old_cell)
			{
				m_MHCLastCell = new_bunch.SE;
				new_bunch.SE->m_MHCNext = nullptr;
			}
			else {
				old_cell->m_MHCNext->m_MHCPrevious = new_bunch.SE;
				new_bunch.SE->m_MHCNext = old_cell->m_MHCNext;
			}
			break;
		case MHC::Profile::U:
			//   .____.     .____.
			//	 | NW |     | NE |
			//   |____|	    |____|
			//	   /\         ||
			//	   ||         \/
			//   .____.     .____.
			//	 | SW | <== | SE |
			//   |____|	    |____|
			// 
			// First cell
			new_bunch.NE->m_MHCIndex = old_cell->m_MHCIndex + (0b00 << mhc_index_offset);
			if (m_MHCFirstCell == old_cell)
			{
				new_bunch.NE->m_MHCPrevious = nullptr;
				m_MHCFirstCell = new_bunch.NE;
			}
			else {
				new_bunch.NE->m_MHCPrevious = old_cell->m_MHCPrevious;
				old_cell->m_MHCPrevious->m_MHCNext = new_bunch.NE;
			}
			new_bunch.NE->m_MHCNext = new_bunch.SE;

			// Second cell
			new_bunch.SE->m_MHCIndex = old_cell->m_MHCIndex + (0b01 << mhc_index_offset);
			new_bunch.SE->m_MHCPrevious = new_bunch.NE;
			new_bunch.SE->m_MHCNext = new_bunch.SW;

			// Third cell
			new_bunch.SW->m_MHCIndex = old_cell->m_MHCIndex + (0b10 << mhc_index_offset);
			new_bunch.SW->m_MHCPrevious = new_bunch.SE;
			new_bunch.SW->m_MHCNext = new_bunch.NW;

			// Last cell
			new_bunch.NW->m_MHCIndex = old_cell->m_MHCIndex + (0b11 << mhc_index_offset);
			new_bunch.NW->m_MHCPrevious = new_bunch.SW;
			new_bunch.NW->m_MHCNext = old_cell->m_MHCNext;
			old_cell->m_MHCNext->m_MHCPrevious = new_bunch.NW;
			break;
		case MHC::Profile::D:
			//   .____.     .____.
			//	 | NW | <== | NE |
			//   |____|	    |____|
			//                /\
			//                ||
			//   .____.     .____.
			//	 | SW | ==> | SE |
			//   |____|	    |____|
			// 
			// First cell
			new_bunch.SW->m_MHCIndex = old_cell->m_MHCIndex + (0b00 << mhc_index_offset);
			new_bunch.SW->m_MHCPrevious = old_cell->m_MHCPrevious;
			old_cell->m_MHCPrevious->m_MHCNext = new_bunch.SW;
			new_bunch.SW->m_MHCNext = new_bunch.SE;

			// Second cell
			new_bunch.SE->m_MHCIndex = old_cell->m_MHCIndex + (0b01 << mhc_index_offset);
			new_bunch.SE->m_MHCPrevious = new_bunch.SW;
			new_bunch.SE->m_MHCNext = new_bunch.NE;

			// Third cell
			new_bunch.NE->m_MHCIndex = old_cell->m_MHCIndex + (0b10 << mhc_index_offset);
			new_bunch.NE->m_MHCPrevious = new_bunch.SE;
			new_bunch.NE->m_MHCNext = new_bunch.NW;

			// Last cell
			new_bunch.NW->m_MHCIndex = old_cell->m_MHCIndex + (0b11 << mhc_index_offset);
			new_bunch.NW->m_MHCPrevious = new_bunch.NE;
			new_bunch.NW->m_MHCNext = old_cell->m_MHCNext;
			old_cell->m_MHCNext->m_MHCPrevious = new_bunch.NW;
			break;
		case MHC::Profile::N:
			//   .____.     .____.
			//	 | NW | ==> | NE |
			//   |____|	    |____|
			//     /\         ||
			//     ||         \/
			//   .____.     .____.
			//	 | SW |     | SE |
			//   |____|	    |____|
			// 
			// First cell
			new_bunch.SW->m_MHCIndex = old_cell->m_MHCIndex + (0b00 << mhc_index_offset);
			new_bunch.SW->m_MHCPrevious = old_cell->m_MHCPrevious;
			old_cell->m_MHCPrevious->m_MHCNext = new_bunch.SW;
			new_bunch.SW->m_MHCNext = new_bunch.NW;

			// Second cell
			new_bunch.NW->m_MHCIndex = old_cell->m_MHCIndex + (0b01 << mhc_index_offset);
			new_bunch.NW->m_MHCPrevious = new_bunch.SW;
			new_bunch.NW->m_MHCNext = new_bunch.NE;

			// Third cell
			new_bunch.NE->m_MHCIndex = old_cell->m_MHCIndex + (0b10 << mhc_index_offset);
			new_bunch.NE->m_MHCPrevious = new_bunch.NW;
			new_bunch.NE->m_MHCNext = new_bunch.SE;

			// Last cell
			new_bunch.SE->m_MHCIndex = old_cell->m_MHCIndex + (0b11 << mhc_index_offset);
			new_bunch.SE->m_MHCPrevious = new_bunch.NE;
			if (m_MHCLastCell == old_cell)
			{
				new_bunch.SE->m_MHCNext = nullptr;
				m_MHCLastCell = new_bunch.SE;
			}
			else {
				new_bunch.SE->m_MHCNext = old_cell->m_MHCNext;
				old_cell->m_MHCNext->m_MHCPrevious = new_bunch.SE;
			}
			break;
		default:
			ALG_CORE_WARN("BuildMHC switch reached default value. (Invalid MHC::Unit)");
			break;
		}
	}


	void Grid2D::ConnectCase2(CellNode* old_cell, CellBunch& new_bunch, TransitionNode* transition, Direction direction)
	{
		// Case 2
		// Neighbouring cells had DIFFERENT levels of refinement before 
		// this refinement.

		// Find where the old cell was connected
		if (NodeType::Cell == transition->m_Lower->GetType()
			&& old_cell == transition->m_Lower)
		{
			// Case 2.1
			// Old cell was connected to the LOWER socket
			ConnectCase21(new_bunch, transition, direction);
		}
		else {
			TransitionNode* new_transition = new TransitionNode;
			new_transition->m_Lower = transition;

			if (
				NodeType::Cell == transition->m_HigherSorE->GetType()
				&& old_cell == transition->m_HigherSorE)
			{
				// Case 2.2 SOUTH_OR_EAST
				// Old cell was connected to the HIGHER SOUTH/EAST socket
				transition->m_HigherSorE = new_transition;
				ConnectCase22(new_bunch, new_transition, direction);
			}
			else if (
				NodeType::Cell == transition->m_HigherNorW->GetType()
				&& old_cell == transition->m_HigherNorW)
			{
				// Case 2.2 NORTH_OR_WEST
				// Old cell was connected to the HIGHER NORTH/WEST socket
				transition->m_HigherNorW = new_transition;
				ConnectCase22(new_bunch, new_transition, direction);
			}
		}
	}


}