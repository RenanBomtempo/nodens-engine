#include "algpch.h"
#include "MHC/MHC.h"
#include "Grid/Grid2D.h"

namespace alg {
	void Grid2D::CoarsenGrid()
	{
		auto cell = m_MHCFirstCell;
		while (cell != nullptr)
		{
			auto next = cell->m_Next;
			cell = CoarsenBunch(cell);
		}

	}

	CellNode* Grid2D::CoarsenBunch(CellNode* cell)
	{
		// Find all cells from same bunch

		CellNode* first, * second, * third, * last;
		
		switch (cell->LocalIndex())
		{
		case 0:
			first = cell;
			second = first->m_Next;
			third = second->m_Next;
			last = third->m_Next;
			break;
		case 1:
			first = cell->m_Previous;
			second = cell;
			third = second->m_Next;
			last = third->m_Next;
			break;
		case 2:
			first = cell->m_Previous->m_Previous;
			second = first->m_Next;
			third = cell;
			last = third->m_Next;
			break;
		case 3:
			first = cell->m_Previous->m_Previous->m_Previous;
			second = first->m_Next;
			third = second->m_Next;
			last = cell;
			break;
		default:
			break;
		}

		switch (MHC::CalculateBunchProfile(cell->m_GlobalIndex, cell->m_RefinementLevel))
		{
		case MHC::Profile::C: 

			break;

		case MHC::Profile::D: 
			break;

		case MHC::Profile::U: 
			break;

		case MHC::Profile::N: 
			break;

		default:
			break;
		}
		return last->m_Next;
	}

	namespace {
		void DisconnectCase1(CellBunch& new_bunch, CellNode* external_cell, Grid2D::Direction direction)
		{
		}

		void DisconnectCase2(CellNode* old_cell_ptr, CellBunch& new_bunch, TransitionNode* transition, Grid2D::Direction direction)
		{
		}

		void DisconnectCase21(CellBunch& new_bunch, TransitionNode* transition, Grid2D::Direction direction)
		{
		}

		void DisconnectCase22(CellBunch& new_bunch, TransitionNode* new_transition, Grid2D::Direction direction)
		{
		}
	}
}