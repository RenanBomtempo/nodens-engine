#include "algpch.h"
#include "Nodes/CellBunch.h"
#include "Grid2D.h"
#include "MHC/mhc.h"
#include "Log.h"


namespace alg {
	Grid2D::Grid2D(std::string title)
		: m_NumberOfCells(0),
		m_Title(title),
		m_NorthBoundary(nullptr),
		m_SouthBoundary(nullptr),
		m_EastBoundary(nullptr),
		m_WestBoundary(nullptr),
		m_MHCFirstCell(nullptr),
		m_MHCLastCell(nullptr)
	{
		alg::Log::Init();
	}

	/**
	* Instantiate the 4 initial cells of the grid.
	*/
	void Grid2D::Initialize()
	{
		ALG_CORE_INFO("Initializing Grid: {0}", m_Title);

		// =====================================================================
		// Instantiate Nodes

		// 4 BOUNDARY TRANSITION nodes
		TransitionNode* north_transition = new TransitionNode();
		TransitionNode* south_transition = new TransitionNode();
		TransitionNode* east_transition = new TransitionNode();
		TransitionNode* west_transition = new TransitionNode();

		// 4 BOUNDARY nodes
		m_NorthBoundary = new BoundaryNode();
		m_SouthBoundary = new BoundaryNode();
		m_EastBoundary = new BoundaryNode();
		m_WestBoundary = new BoundaryNode();

		// 4 INITIAL CELL nodes
		CellBunch initial_cells;
		initial_cells.Init(Vector2D(0.5), 1);

		// =====================================================================
		// Connect BoundaryNodes

		m_NorthBoundary->m_TransitionNode = north_transition;
		north_transition->m_Lower = m_NorthBoundary;

		m_SouthBoundary->m_TransitionNode = south_transition;
		south_transition->m_Lower = m_SouthBoundary;

		m_EastBoundary->m_TransitionNode = east_transition;
		east_transition->m_Lower = m_EastBoundary;

		m_WestBoundary->m_TransitionNode = west_transition;
		west_transition->m_Lower = m_WestBoundary;


		// =====================================================================
		// Connect TransitionNodes

		//   /\
		// NW  NE 
		//         
		// SW  SE 
		initial_cells.NW->m_North = north_transition;
		initial_cells.NE->m_North = north_transition;
		north_transition->m_HigherNorW = initial_cells.NW;
		north_transition->m_HigherSorE = initial_cells.NE;

		// NW  NE 
		//        >
		// SW  SE  
		initial_cells.NE->m_East = east_transition;
		initial_cells.SE->m_East = east_transition;
		east_transition->m_HigherNorW = initial_cells.NE;
		east_transition->m_HigherSorE = initial_cells.SE;

		// NW  NE 
		//         
		// SW  SE  
		//   \/
		initial_cells.SW->m_South = south_transition;
		initial_cells.SE->m_South = south_transition;
		south_transition->m_HigherNorW = initial_cells.SW;
		south_transition->m_HigherSorE = initial_cells.SE;

		//   NW  NE 
		// <  
		//   SW  SE  
		initial_cells.SW->m_West = west_transition;
		initial_cells.NW->m_West = west_transition;
		west_transition->m_HigherNorW = initial_cells.NW;
		west_transition->m_HigherSorE = initial_cells.SW;


		// =====================================================================
		//	Initial MHC Ordering
		//   .____.     .____.
		//	 | NW | <== | NE |
		//   |____|	    |____|
		//     ||
		//     \/
		//   .____.     .____.
		//	 | SW | ==> | SE |
		//   |____|	    |____|

		m_MHCFirstCell = initial_cells.NE;
		initial_cells.NE->m_MHCIndex = 0b00;
		initial_cells.NE->m_MHCPrevious = nullptr;
		initial_cells.NE->m_MHCNext = initial_cells.NW;

		initial_cells.NW->m_MHCIndex = 0b01;
		initial_cells.NW->m_MHCPrevious = initial_cells.NE;
		initial_cells.NW->m_MHCNext = initial_cells.SW;

		initial_cells.SW->m_MHCIndex = 0b10;
		initial_cells.SW->m_MHCPrevious = initial_cells.NW;
		initial_cells.SW->m_MHCNext = initial_cells.SE;

		initial_cells.SE->m_MHCIndex = 0b11;
		initial_cells.SE->m_MHCPrevious = initial_cells.SW;
		initial_cells.SE->m_MHCNext = nullptr;
		m_MHCLastCell = initial_cells.SE;

		m_NumberOfCells = 4;

		ALG_CORE_INFO("2D Grid initialized");
	}

	Grid2D::~Grid2D()
	{
		ALG_CORE_INFO("Deleting 2D Grid");

		// Detach BoundaryNodes from the outer TransitionNodes
		m_NorthBoundary->m_TransitionNode->m_Lower = nullptr;
		m_SouthBoundary->m_TransitionNode->m_Lower = nullptr;
		m_EastBoundary->m_TransitionNode->m_Lower = nullptr;
		m_WestBoundary->m_TransitionNode->m_Lower = nullptr;

		delete m_NorthBoundary;
		delete m_SouthBoundary;
		delete m_EastBoundary;
		delete m_WestBoundary;

		CellNode* curr = m_MHCFirstCell;
		CellNode* next = curr->m_MHCNext;
		while (curr != nullptr) {
			next = curr->m_MHCNext;
			delete curr;
			curr = next;
		}

		ALG_CORE_INFO("2D Grid deleted");
	}

	void Grid2D::Print()
	{
		std::cout << "| '" << m_Title << "'" << std::endl;
		std::cout << "| --------------------------------" << std::endl;
		CellNode* curr = m_MHCFirstCell;

		while (curr != nullptr) {
			curr->Print();
			curr = curr->m_MHCNext;
		}
	}
}