#include "algpch.h"
#include "Log.h"
#include "Grid/Grid2D.h"
#include "Grid/MHC/MHC.h"

namespace alg {
	/**
	* /brief Refines the entire grid.
	*
	* /returns A reference to the first cell in the new bunch.
	*/
	void Grid2D::RefineGrid()
	{
		if (m_NumberOfCells == (0b1 << (2 * MAX_REFINEMENT_LEVEL))) {
			ALG_CORE_ERROR("Grid is already fully refined!");
			return;
		}

		auto cell = m_FirstCell;
		while (cell != nullptr)
		{
			auto next = cell->Next();
			try { RefineCell(cell); }
			catch (std::runtime_error& e) {}
			cell = next;
		}
	}

	/**
	* /brief Refine a cell and returns a reference to the first cell from
	* the newly created cell bunch.
	*
	* /param old_cell: The cell to be refined.
	*
	* /returns A reference to the first cell in the new bunch.
	*/
	CellBunch Grid2D::RefineCell(CellNode* old_cell)
	{
		ALG_CORE_ASSERT(m_NumberOfCells > 0, "Grid has not been initialized!");
		ALG_CORE_ASSERT(old_cell != nullptr, "RefineCell: 'old_cell' is nullptr.");

		ALG_CORE_INFO("Refining cell {0}", old_cell->GlobalIndexAsBinaryString());

		if (old_cell->m_RefinementLevel == MAX_REFINEMENT_LEVEL) 
			throw std::runtime_error("Cell has reached maximum refinement level.");
		

		// =====================================================================
		// Connect Neighbors

		uint8_t new_refinement_level = old_cell->m_RefinementLevel + 1;
		CellBunch new_bunch;
		new_bunch.Initialize(old_cell->m_Center, new_refinement_level);

		//______________________________________________________________________
		// Connect North
		switch (old_cell->m_North->GetType())
		{
		case NodeType::Cell:
			_RefineCase1(new_bunch, (CellNode*)old_cell->m_North, Direction::North);
			break;
		case NodeType::Transition:
			_RefineCase2(old_cell, new_bunch, (TransitionNode*)(old_cell->m_North), Direction::North);
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
			_RefineCase1(new_bunch, (CellNode*)(old_cell->m_South), Direction::South);
			break;
		case NodeType::Transition:
			_RefineCase2(old_cell, new_bunch, (TransitionNode*)(old_cell->m_South), Direction::South);
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
			_RefineCase1(new_bunch, (CellNode*)(old_cell->m_West), Direction::West);
			break;
		case NodeType::Transition:
			_RefineCase2(old_cell, new_bunch, (TransitionNode*)(old_cell->m_West), Direction::West);
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
			_RefineCase1(new_bunch, (CellNode*)(old_cell->m_East), Direction::East);
			break;
		case NodeType::Transition:
			_RefineCase2(old_cell, new_bunch, (TransitionNode*)(old_cell->m_East), Direction::East);
			break;
		default:
			ALG_ASSERT(false, "Invalid Cell Type");
			break;
		}

		// =====================================================================
		// Update MHC Ordering
		_UpdateMHCAfterRefinement(old_cell, new_bunch);
		//CellNode* first_cell_in_new_bunch = old_cell->m_MHCPrevious->m_MHCNext;

		// =====================================================================
		// Process Data Payload
		// 
		// Call the user defined function responsible for processing the 
		// datapayload inside the newly created cells. 

		this->m_NumberOfCells += 3;

		ALG_CORE_INFO("Cell {0} succesfully refined to level {1}", old_cell->GlobalIndexAsBinaryString(), new_refinement_level);

		// Isolate cell before deleting it
		old_cell->m_North = nullptr;
		old_cell->m_East = nullptr;
		old_cell->m_South = nullptr;
		old_cell->m_West = nullptr;
		delete old_cell;


		return new_bunch;
	}

	/* Case 1
	*  Before refining, neighbouring cells HAD THE SAME LEVEL OF REFINEMENT
	 *    _____            _____
	 *   |     |          |     |
	 *   |_____|          |_____|
	 *      |                |
	 *      |       =>       T
	 *    __|__            _/_\_
	 *   |     |          |__|__|
	 *   |_____|          |__|__|
	 *
	 */
	void Grid2D::_RefineCase1(CellBunch& new_bunch, CellNode* external_cell, Direction direction)
	{
		TransitionNode* new_transition = new TransitionNode();

		external_cell->Connect(Opposite(direction), new_transition);
		new_bunch.ConnectToTransition(direction, new_transition);

		// Connect Transition Node
		new_transition->m_Lower = external_cell;
		switch (direction)
		{
		case Direction::North:
			new_transition->m_HigherNorW = new_bunch.NW;
			new_transition->m_HigherSorE = new_bunch.NE;
			break;

		case Direction::South:
			new_transition->m_HigherNorW = new_bunch.SW;
			new_transition->m_HigherSorE = new_bunch.SE;
			break;

		case Direction::West:
			new_transition->m_HigherNorW = new_bunch.NW;
			new_transition->m_HigherSorE = new_bunch.SW;
			break;

		case Direction::East:
			new_transition->m_HigherNorW = new_bunch.NE;
			new_transition->m_HigherSorE = new_bunch.SE;
			break;

		default:
			break;
		}
	}

	/* Case 2
	 *  Neighbouring cells had DIFFERENT levels of refinement before refinement.
	 *
	 *                            ?     ?
	 *	                           \   /
	 *	                             T
	 *	                       ._____|_____.
	 *	                       |...........|
	 *
	 */
	void Grid2D::_RefineCase2(CellNode* old_cell, CellBunch& new_bunch, TransitionNode* transition, Direction direction)
	{
		// Find where the old cell was connected
		if (old_cell == transition->m_Lower)
		{
			/* Case 2.1
			 * Old cell was connected to the LOWER port
			 *
			 *
			 *	          ?   ?                       ?      ?
			 *	           \ /   		             /      /
			 *	            T			  =>        /      /
			 *	   ________/________	       ____/______/_____
			 *	  /                /	      /       /        /
			 *	 /                /		     /_______/________/
			 *	/. . . . . . . . /          /. . . ./. . . . /
			 */
			_RefineCase2a(new_bunch, transition, direction);
		}
		else {
			TransitionNode* new_transition = new TransitionNode;
			new_transition->m_Lower = transition;

			_RefineCase2b(new_bunch, new_transition, direction);

			/* Case 2.2 SOUTH_OR_EAST
			* Old cell was connected to the HIGHER SOUTH/EAST port
			*      ...........         ...........
			*     |_____|_____|       |_____|_____|
			*     |     |     |       |     |__|__|
			*     |_____|_____|       |_____|__|__|
			*        \     /             \    \ /
			*         \   /	              \    T
			*           T	      =>       \ /
			*           |	                T
			*     ._____|_____.       ._____|_____.
			*     |...........|       |...........|
			*/
			if (old_cell == transition->m_HigherSorE)
				transition->m_HigherSorE = new_transition;

			/* Case 2.2 NORTH_OR_WEST
			* Old cell was connected to the HIGHER NORTH_OR_WEST port
			*      ...........         ...........
			*     |_____|_____|       |_____|_____|
			*     |     |     |       |__|__|     |
			*     |_____|_____|       |__|__|_____|
			*        \     /            \ /    /
			*         \   /	             T    /
			*           T	      =>       \ /
			*           |	                T
			*     ._____|_____.       ._____|_____.
			*     |...........|       |...........|
			*/
			else if (old_cell == transition->m_HigherNorW)
				transition->m_HigherNorW = new_transition;
		}
	}

	void Grid2D::_RefineCase2a(CellBunch& new_bunch, TransitionNode* transition, Direction direction)
	{
		// Connect HIGHER SOUTH OR EAST
		if (NodeType::Cell == transition->m_HigherSorE->GetType())
		{
			/* Case 2.1.1
			 * Higher SorE connects directly to another cell
			 *                      . . . . .                         . . . . .
			 *                     /_______/                         /_______/
			 *                    /       /                         /       /
			 *              ?    /_______/	                  ?    /_______/
			 *              \      /		                 /       /
			 *               \   /			                /       /
			 *                 T			  =>           /       /
			 *                /				              /       /
			 *      ________/________		         ____/_______/____
			 *     /                /		        /       /        /
			 *    /                /		       /_______/________/
			 *   /. . . . . . . . /               /. . . ./. . . . /
			 */
			CellNode* externalCell = (CellNode*)transition->m_HigherSorE;

			switch (direction)
			{
				// EAST
			case Direction::North:
				new_bunch.NE->m_North = externalCell;
				externalCell->m_South = new_bunch.NE;
				break;
			case Direction::South:
				new_bunch.SE->m_South = externalCell;
				externalCell->m_North = new_bunch.SE;
				break;

				// SOUTH
			case Direction::West:
				new_bunch.SW->m_West = externalCell;
				externalCell->m_East = new_bunch.SW;
				break;
			case Direction::East:
				new_bunch.SE->m_East = externalCell;
				externalCell->m_West = new_bunch.SE;
				break;
			default:
				break;
			}
		}
		else if (NodeType::Transition == transition->m_HigherSorE->GetType())
		{
			/*
			 * Case 2.1.2
			 * Transition SorE connects to another transition
			 *
			 *                   ?   ?                              ?   ?
			 *                    \ /                                \ /
			 *              ?      T	                     ?        T
			 *              \     /		                     /       /
			 *                T			    =>             /       /
			 *      ________/________		         ____/_______/____
			 *     /                /		        /       /        /
			 *    /                /		       /_______/________/
			 *   /. . . . . . . . /               /. . . ./. . . . /
			 */
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
			/*
			// Case 2.1.1
			// Transition NorW connects directly to another cell
			 *             . . . . .                         . . . . .
			 *            /_______/                         /_______/
			 *           /       /                         /       /
			 *          /_______/	?                     /_______/	  ?
			 *              \     /		                     /       /
			 *               \  /			                /       /
			 *                T			  =>               /       /
			 *               /				              /       /
			 *      ________/________		         ____/_______/____
			 *     /                /		        /       /        /
			 *    /                /		       /_______/________/
			 *   /. . . . . . . . /               /. . . ./. . . . /
			 */
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
			/*
			// Case 2.1.2
			// Transition NorW connects to another transition
			 *
			 *           ?   ?                             ?   ?
			 *            \ /                               \ /
			 *             T	 ?                           T       ?
			 *               \  /			                /       /
			 *                T			    =>             /       /
			 *               /				              /       /
			 *      ________/________		         ____/_______/____
			 *     /                /		        /       /        /
			 *    /                /		       /_______/________/
			 *   /. . . . . . . . /               /. . . ./. . . . /
			 */
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

	void Grid2D::_RefineCase2b(CellBunch& new_bunch, TransitionNode* new_transition, Direction direction)
	{
		new_bunch.ConnectToTransition(direction, new_transition);

		switch (direction)
		{
		case Direction::North:
			new_transition->m_HigherSorE = new_bunch.NE;
			new_transition->m_HigherNorW = new_bunch.NW;
			break;
		case Direction::South:
			new_transition->m_HigherSorE = new_bunch.SE;
			new_transition->m_HigherNorW = new_bunch.SW;
			break;
		case Direction::West:
			new_transition->m_HigherNorW = new_bunch.NW;
			new_transition->m_HigherSorE = new_bunch.SW;
			break;
		case Direction::East:
			new_transition->m_HigherNorW = new_bunch.NE;
			new_transition->m_HigherSorE = new_bunch.SE;
			break;
		default:
			throw std::runtime_error("Invalid direction.");
			break;
		}
	}

	void Grid2D::_UpdateMHCAfterRefinement(CellNode* old_cell, CellBunch& new_bunch)
	{
		ALG_CORE_ASSERT(old_cell != nullptr, "UpdateMHC: 'old_cell' is nullptr.");

		uint32_t mhc_index_offset = 2 * (old_cell->m_RefinementLevel);

		switch (MHC::CalculateBunchProfile(old_cell->m_GlobalIndex, old_cell->m_RefinementLevel + 1))
		{
		case MHC::BunchProfile::C:
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
			new_bunch.NE->m_GlobalIndex = old_cell->m_GlobalIndex + (0b00 << mhc_index_offset);
			if (m_FirstCell == old_cell)
			{
				new_bunch.NE->m_Previous = nullptr;
				m_FirstCell = new_bunch.NE;
			}
			else {
				new_bunch.NE->m_Previous = old_cell->m_Previous;
				old_cell->m_Previous->m_Next = new_bunch.NE;
			}
			new_bunch.NE->m_Next = new_bunch.NW;

			// Second cell
			new_bunch.NW->m_GlobalIndex = old_cell->m_GlobalIndex + (0b01 << mhc_index_offset);
			new_bunch.NW->m_Previous = new_bunch.NE;
			new_bunch.NW->m_Next = new_bunch.SW;

			// Third cell
			new_bunch.SW->m_GlobalIndex = old_cell->m_GlobalIndex + (0b10 << mhc_index_offset);
			new_bunch.SW->m_Previous = new_bunch.NW;
			new_bunch.SW->m_Next = new_bunch.SE;

			// Last cell
			new_bunch.SE->m_GlobalIndex = old_cell->m_GlobalIndex + (0b11 << mhc_index_offset);
			new_bunch.SE->m_Previous = new_bunch.SW;
			if (m_LastCell == old_cell)
			{
				m_LastCell = new_bunch.SE;
				new_bunch.SE->m_Next = nullptr;
			}
			else {
				old_cell->m_Next->m_Previous = new_bunch.SE;
				new_bunch.SE->m_Next = old_cell->m_Next;
			}
			break;
		case MHC::BunchProfile::U:
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
			new_bunch.NE->m_GlobalIndex = old_cell->m_GlobalIndex + (0b00 << mhc_index_offset);
			if (m_FirstCell == old_cell)
			{
				new_bunch.NE->m_Previous = nullptr;
				m_FirstCell = new_bunch.NE;
			}
			else {
				new_bunch.NE->m_Previous = old_cell->m_Previous;
				old_cell->m_Previous->m_Next = new_bunch.NE;
			}
			new_bunch.NE->m_Next = new_bunch.SE;

			// Second cell
			new_bunch.SE->m_GlobalIndex = old_cell->m_GlobalIndex + (0b01 << mhc_index_offset);
			new_bunch.SE->m_Previous = new_bunch.NE;
			new_bunch.SE->m_Next = new_bunch.SW;

			// Third cell
			new_bunch.SW->m_GlobalIndex = old_cell->m_GlobalIndex + (0b10 << mhc_index_offset);
			new_bunch.SW->m_Previous = new_bunch.SE;
			new_bunch.SW->m_Next = new_bunch.NW;

			// Last cell
			new_bunch.NW->m_GlobalIndex = old_cell->m_GlobalIndex + (0b11 << mhc_index_offset);
			new_bunch.NW->m_Previous = new_bunch.SW;
			new_bunch.NW->m_Next = old_cell->m_Next;
			old_cell->m_Next->m_Previous = new_bunch.NW;
			break;
		case MHC::BunchProfile::D:
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
			new_bunch.SW->m_GlobalIndex = old_cell->m_GlobalIndex + (0b00 << mhc_index_offset);
			new_bunch.SW->m_Previous = old_cell->m_Previous;
			old_cell->m_Previous->m_Next = new_bunch.SW;
			new_bunch.SW->m_Next = new_bunch.SE;

			// Second cell
			new_bunch.SE->m_GlobalIndex = old_cell->m_GlobalIndex + (0b01 << mhc_index_offset);
			new_bunch.SE->m_Previous = new_bunch.SW;
			new_bunch.SE->m_Next = new_bunch.NE;

			// Third cell
			new_bunch.NE->m_GlobalIndex = old_cell->m_GlobalIndex + (0b10 << mhc_index_offset);
			new_bunch.NE->m_Previous = new_bunch.SE;
			new_bunch.NE->m_Next = new_bunch.NW;

			// Last cell
			new_bunch.NW->m_GlobalIndex = old_cell->m_GlobalIndex + (0b11 << mhc_index_offset);
			new_bunch.NW->m_Previous = new_bunch.NE;
			new_bunch.NW->m_Next = old_cell->m_Next;
			old_cell->m_Next->m_Previous = new_bunch.NW;
			break;
		case MHC::BunchProfile::N:
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
			new_bunch.SW->m_GlobalIndex = old_cell->m_GlobalIndex + (0b00 << mhc_index_offset);
			new_bunch.SW->m_Previous = old_cell->m_Previous;
			old_cell->m_Previous->m_Next = new_bunch.SW;
			new_bunch.SW->m_Next = new_bunch.NW;

			// Second cell
			new_bunch.NW->m_GlobalIndex = old_cell->m_GlobalIndex + (0b01 << mhc_index_offset);
			new_bunch.NW->m_Previous = new_bunch.SW;
			new_bunch.NW->m_Next = new_bunch.NE;

			// Third cell
			new_bunch.NE->m_GlobalIndex = old_cell->m_GlobalIndex + (0b10 << mhc_index_offset);
			new_bunch.NE->m_Previous = new_bunch.NW;
			new_bunch.NE->m_Next = new_bunch.SE;

			// Last cell
			new_bunch.SE->m_GlobalIndex = old_cell->m_GlobalIndex + (0b11 << mhc_index_offset);
			new_bunch.SE->m_Previous = new_bunch.NE;
			if (m_LastCell == old_cell)
			{
				new_bunch.SE->m_Next = nullptr;
				m_LastCell = new_bunch.SE;
			}
			else {
				new_bunch.SE->m_Next = old_cell->m_Next;
				old_cell->m_Next->m_Previous = new_bunch.SE;
			}
			break;
		default:
			ALG_CORE_ERROR("Invalid MHC Profile");
			break;
		}
	}
}