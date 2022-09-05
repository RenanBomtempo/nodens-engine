#pragma once
#include "Core.h"
#include "Grid/Direction.h"
#include "Grid/Nodes/CellNode.h"
#include "Grid/Nodes/TransitionNode.h"
#include "Grid/Nodes/BoundaryNode.h"
#include "Grid/Nodes/CellBunch.h"

namespace alg {
	/**
		Unit square grid with origin located at the lower left corner.
	*/

	class Grid2D {
	public:
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
		void _UpdateMHCAfterCoarsening(CellNode* new_cell, CellBunch& old_bunch);

		void _RefineCase1(CellBunch& new_bunch, CellNode* external_cell, Direction direction);
		void _RefineCase2(CellNode* old_cell_ptr, CellBunch& new_bunch, TransitionNode* transition, Direction direction);
		void _RefineCase2a(CellBunch& new_bunch, TransitionNode* transition, Direction direction);
		void _RefineCase2b(CellBunch& new_bunch, TransitionNode* new_transition, Direction direction);
		
		void _CoarsenCase1(TransitionNode* old_transition, CellNode* new_cell, Direction direction);
		void _CoarsenCase2(Node* outer_cell_SorE, Node* outer_cell_NorW, CellNode* new_cell, Direction direction);
		void _CoarsenCase2a(Node* outer_cell, CellNode* new_cell, TransitionNode* new_transition, Direction direction);

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