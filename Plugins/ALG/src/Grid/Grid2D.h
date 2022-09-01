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
		CellNode* const RefineCell(CellNode* old_cell);
		CellNode* const CoarsenBunch(CellNode* cell);

		CellNode* FirstCell() const { return m_MHCFirstCell; }
		CellNode* LastCell() const { return m_MHCLastCell; }
		uint16_t CellCount() const { return m_NumberOfCells; }
		
		void Print();

	private:
		void UpdateMHC(CellNode* old_cell, CellBunch& new_bunch);

		// TODO: Move these functions to the implementation file only 
		void ConnectCase1(CellBunch& new_bunch, CellNode* external_cell, Direction direction);
		void ConnectCase2(CellNode* old_cell_ptr, CellBunch& new_bunch, TransitionNode* transition, Direction direction);
		void ConnectCase21(CellBunch& new_bunch, TransitionNode* transition, Direction direction);
		void ConnectCase22(CellBunch& new_bunch, TransitionNode* new_transition, Direction direction);

		CellNode* FindFirstCellInBunch(CellNode* cell);

		// Modified Hilbert's Curve Ordering
		CellNode* m_MHCFirstCell;
		CellNode* m_MHCLastCell;
		
		BoundaryNode* m_NorthBoundary;
		BoundaryNode* m_SouthBoundary;
		BoundaryNode* m_EastBoundary;
		BoundaryNode* m_WestBoundary;

		uint16_t m_NumberOfCells;

		std::string m_Title;
	};
}