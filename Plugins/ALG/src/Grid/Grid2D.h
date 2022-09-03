#pragma once
#include "Core.h"
#include "Nodes/CellNode.h"
#include "Nodes/TransitionNode.h"
#include "Nodes/BoundaryNode.h"
#include "Nodes/CellBunch.h"

namespace alg {
	/**
		Unit square grid with origin located at the lower left corner.
	*/
	class Grid2D {
	public:
		enum class Direction {
			North, South, West, East
		};

		Grid2D(std::string title = "ALG 2D Grid");
		~Grid2D();

		void Initialize();
		// TODO: void SetCellNodeData();
		// TODO: void SetTransitionNodeData();
		// TODO: void SetBoundaryNodeData();

		void RefineGrid();
		void CoarsenGrid();
		CellBunch RefineCell(CellNode* old_cell) ;
		CellNode* CoarsenBunch(CellBunch& old_bunch);

		CellNode* FirstCell() const { return m_FirstCell; }
		CellNode* LastCell() const { return m_LastCell; }
		uint16_t CellCount() const { return m_NumberOfCells; }
		
		void Print();

	private:
		void _UpdateMHCAfterRefinement(CellNode* old_cell, CellBunch& new_bunch);
		void _UpdateMHCAfterCoarsening(CellNode* new_cell, CellNode* old_first_cell, CellNode* old_last_cell);

		void _RefineCase1(CellBunch& new_bunch, CellNode* external_cell, Direction direction);
		void _RefineCase2(CellNode* old_cell_ptr, CellBunch& new_bunch, TransitionNode* transition, Direction direction);
		void _RefineCase2a(CellBunch& new_bunch, TransitionNode* transition, Direction direction);
		void _RefineCase2b(CellBunch& new_bunch, TransitionNode* new_transition, Direction direction);
		
		void _CoarsenCase2(CellNode* outer_cell_SorE, CellNode* outer_cell_NorW, CellNode* new_cell, Grid2D::Direction direction);
		void _CoarsenCase1(CellNode* outer_cell, CellNode* new_cell, Grid2D::Direction direction);
		void _CoarsenCase2a(CellBunch& bunch, CellNode* new_cell, Grid2D::Direction direction);
		void _CoarsenCase2b(CellBunch& bunch, CellNode* new_cell, Grid2D::Direction direction);

		// Modified Hilbert's Curve Ordering
		CellNode* m_FirstCell;
		CellNode* m_LastCell;
		
		BoundaryNode* m_NorthBoundary;
		BoundaryNode* m_SouthBoundary;
		BoundaryNode* m_EastBoundary;
		BoundaryNode* m_WestBoundary;

		uint16_t m_NumberOfCells;

		std::string m_Title;
	};
}