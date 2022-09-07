#include "algpch.h"
#include "Grid/Grid2D.h"
#include "Grid/Direction.h"
#include "Grid/MHC/MHC.h"

namespace alg {
	void Grid2D::CoarsenGrid()
	{
		auto cell = m_FirstCell;
		while (cell != nullptr)
		{
			try { cell = CoarsenBunch(cell->GetBunch()); }
			catch (std::runtime_error& e) {}
			cell = cell->Next();
		}
	}

	CellNode* Grid2D::CoarsenBunch(CellBunch& old_bunch)
	{
		if (old_bunch.first->m_RefinementLevel == 1)
			throw std::runtime_error("Cant coarsen initial bunch");


		CellNode* newCell = new CellNode();
		newCell->m_Center = old_bunch.CalculateCenter();

		//______________________________________________________________________
		// Reconnect North
		if (old_bunch.NE->m_North == old_bunch.NW->m_North)
			_CoarsenCase1(
				(TransitionNode*)old_bunch.NE->m_North,
				newCell, Direction::North);
		else
			_CoarsenCase2(
				old_bunch.NE->m_North, old_bunch.NW->m_North,
				newCell, Direction::North);

		//______________________________________________________________________
		// Connect South
		if (old_bunch.SE->m_South == old_bunch.SW->m_South)
			_CoarsenCase1(
				(TransitionNode*)old_bunch.SE->m_South,
				newCell, Direction::South);
		else
			_CoarsenCase2(
				old_bunch.SE->m_South, old_bunch.SW->m_South,
				newCell, Direction::South);

		//______________________________________________________________________
		// Connect West
		if (old_bunch.NW->m_West == old_bunch.SW->m_West)
			_CoarsenCase1(
				(TransitionNode*)old_bunch.SW->m_West,
				newCell, Direction::West);
		else
			_CoarsenCase2(
				old_bunch.SW->m_West, old_bunch.NW->m_West,
				newCell, Direction::West);

		//______________________________________________________________________
		// Connect East
		if (old_bunch.NE->m_East == old_bunch.SE->m_East)
			_CoarsenCase1(
				(TransitionNode*)old_bunch.NE->m_East,
				newCell, Direction::East);
		else
			_CoarsenCase2(
				old_bunch.SE->m_East, old_bunch.NE->m_East,
				newCell, Direction::East);


		// MHC Ordering
		_UpdateMHCAfterCoarsening(newCell, old_bunch);

		m_NumberOfCells -= 3;

		old_bunch.Destroy();

		return newCell;
	}

	/* Old bunch is connected to a transition node.
	*
	*
	*          ?                       ?
	*         /                       /
	*        T           =>          /
	*    ___/_\__               ____/___
	*   /___/___/              /       /
	*  /___/___/              /_______/
	*
	*/
	void Grid2D::_CoarsenCase1(
		TransitionNode* old_transition, CellNode* new_cell, Direction direction)
	{
		NodeType lowerNodeType = old_transition->m_Lower->GetType();
		switch (lowerNodeType)
		{
		case NodeType::Transition:
		{
			TransitionNode* secondTransition = (TransitionNode*)old_transition->m_Lower;

			new_cell->Connect(direction, secondTransition);

			if (old_transition == secondTransition->m_HigherNorW)
				/*
				 *        ?				         ?
				 *        |				         |
				 *        T				         T
				 *      /   \       =>	       /   \
				 *     T     ? 			      /     ?
				 *   _/_\_    			   __/__
				 *  |__|__|   			  |     |
				 *  |__|__|				  |_____|
				 *
				 */
				secondTransition->m_HigherNorW = new_cell;

			else if (old_transition == secondTransition->m_HigherSorE)
				/*
				 *        ?			          ?
				 *        |			          |
				 *        T			          T
				 *      /   \       =>      /   \
				 *     ?     T             /     \
				 *         _/_\_    	   ?     __\__
				 *        |__|__|   	        |     |
				 *        |__|__|               |_____|
				 *
				 */
				secondTransition->m_HigherSorE = new_cell;
		}
		break;
		case NodeType::Cell:
			/* Old bunch is connected to a transition node.
			 *      _____		     _____
			 *     |     |		    |     |
			 *     |_____|		    |_____|
			 *        |                |
			 *        T       =>       |
			 *      _/_\_    	     __|__
			 *     |__|__|   	    |     |
			 *     |__|__|		    |_____|
			 *
			 */
		{
			CellNode* externalCell = (CellNode*)old_transition->m_Lower;
			externalCell->Connect(Opposite(direction), new_cell);
			new_cell->Connect(direction, externalCell);
		}
		break;
		case NodeType::Boundary:
			break;
		default:
			break;
		}

		delete(old_transition);
	}

	/* Group is connected to another bunch of the same refinement level.
	*
	*                    ?   ?                ?   ?
	*                   /   /                  \ /
	*                  /   /        =>          T
	*               __/___/_               ___ /___
	*              /___/___/              /       /
	*             /___/___/              /_______/
	*
	*/
	void Grid2D::_CoarsenCase2(
		Node* neighbor_node_SorE, Node* neighbor_node_NorW,
		CellNode* new_cell, Direction direction)
	{
		// Create Transition node
		TransitionNode* newTransition = new TransitionNode();
		newTransition->m_Lower = new_cell;
		new_cell->Connect(direction, newTransition);

		newTransition->m_HigherNorW = neighbor_node_NorW;
		_CoarsenCase2a(neighbor_node_NorW, new_cell, newTransition, direction);

		newTransition->m_HigherSorE = neighbor_node_SorE;
		_CoarsenCase2a(neighbor_node_SorE, new_cell, newTransition, direction);
	}

	void Grid2D::_CoarsenCase2a(Node* outer_cell, CellNode* new_cell, TransitionNode* new_transition, Direction direction)
	{
		if (NodeType::Cell == outer_cell->GetType())
			((CellNode*)outer_cell)->Connect(Opposite(direction), new_transition);
		else if (NodeType::Transition == outer_cell->GetType())
			((TransitionNode*)outer_cell)->m_Lower = new_transition;
	}

	void Grid2D::_UpdateMHCAfterCoarsening(
		CellNode* new_cell, CellBunch& old_bunch)
	{
		new_cell->m_GlobalIndex = old_bunch.first->GlobalBunchIndex();
		new_cell->m_RefinementLevel = old_bunch.first->m_RefinementLevel - 1;

		new_cell->m_Previous = old_bunch.first->m_Previous;
		new_cell->m_Next = old_bunch.last->m_Next;

		if (old_bunch.last->HasNext())
			old_bunch.last->m_Next->m_Previous = new_cell;

		if (old_bunch.first->HasPrevious())
			old_bunch.first->m_Previous->m_Next = new_cell;

		if (old_bunch.first == FirstCell())
			m_FirstCell = new_cell;
		else if (old_bunch.last == LastCell())
			m_LastCell = new_cell;
	}
}