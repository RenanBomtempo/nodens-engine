#include "algpch.h"
#include "Grid/MHC/MHC.h"
#include "CellBunch.h"
#include "CellNode.h"
#include "Log.h"

namespace alg {
	NodeType CellNode::m_Type = NodeType::Cell;

	CellNode::CellNode()
		: m_North(nullptr),
		m_East(nullptr),
		m_South(nullptr),
		m_West(nullptr),
		m_Next(nullptr),
		m_Previous(nullptr),
		m_RefinementLevel(-1),
		m_Center(Vector2D(0)),
		m_GlobalIndex(-1)
	{
	}

	CellNode::~CellNode()
	{
	}



	std::string CellNode::BunchIndexAsBinaryString()
	{
		std::string str = std::bitset<2 * MAX_REFINEMENT_LEVEL>(BunchIndex()).to_string();
		for (int i = 0; i < MAX_REFINEMENT_LEVEL - 1; i++)
			str.insert(3 * i + 2, 1, '.');
		return str;
	}

	std::string CellNode::GlobalIndexAsBinaryString()
	{
		std::string str = std::bitset<2 * MAX_REFINEMENT_LEVEL>(m_GlobalIndex).to_string();
		for (int i = 0; i < MAX_REFINEMENT_LEVEL - 1; i++)
			str.insert(3 * i + 2, 1, '.');
		return str;
	}

	uint32_t CellNode::GlobalIndex()
	{
		return m_GlobalIndex;
	}

	uint32_t CellNode::LocalIndex()
	{
		return m_GlobalIndex >> (2 * (m_RefinementLevel - 1));
	}

	uint32_t CellNode::BunchIndex()
	{
		uint32_t mask = 0;
		for (int i = 0; i < m_RefinementLevel - 1; i++)
			mask += 0b11 << (2 * i);

		return (m_GlobalIndex & mask);
	}
	bool CellNode::HasNext()
	{
		return m_Next != nullptr;
	}

	bool CellNode::HasPrevious()
	{
		return m_Previous != nullptr;
	}

	CellNode* CellNode::Next()
	{
		return m_Next;
	}

	CellNode* CellNode::Previous()
	{
		return m_Previous;
	}

	void CellNode::Disconnect(Node* other)
	{
		if (m_North == other) m_North = nullptr;
		if (m_East == other) m_East = nullptr;
		if (m_South == other) m_South = nullptr;
		if (m_West == other) m_West = nullptr;
	}

	void CellNode::Print()
	{
		std::cout << "| Cell " << GlobalIndexAsBinaryString();
		std::cout << std::endl;

		std::cout << "|\tCenter= (";
		std::cout << Center().x << ", " << Center().y;
		std::cout << ")";
		std::cout << std::endl;

		std::cout << "|\tRefLevel= " << (unsigned short)RefinementLevel();
		std::cout << std::endl;
	}

	CellBunch CellNode::GetBunch() {
		CellNode* cell_1, * cell_2, * cell_3, * cell_4;
		switch (LocalIndex())
		{
		case 0:
			// is the first of the bunch
			cell_1 = this;
			cell_2 = cell_1->m_Next;
			cell_3 = cell_2->m_Next;
			cell_4 = cell_3->m_Next;
			break;

		case 1:
			// is the second cell of the bunch
			cell_1 = m_Previous;
			cell_2 = this;
			cell_3 = cell_2->m_Next;
			cell_4 = cell_3->m_Next;
			break;

		case 2:
			// is the third cell of the bunch
			cell_1 = m_Previous->m_Previous;
			cell_2 = cell_1->m_Next;
			cell_3 = this;
			cell_4 = cell_3->m_Next;
			break;

		case 3:
			// is the last cell of the bunch
			cell_1 = m_Previous->m_Previous->m_Previous;
			cell_2 = cell_1->m_Next;
			cell_3 = cell_2->m_Next;
			cell_4 = this;
			break;

		default:
			ALG_ASSERT(false, "Invalid local cell index.")
				break;
		}

		// Store cells in a bunch struct to facilitate pointer manipulation
		CellBunch bunch;
		bunch.first = cell_1;
		bunch.last = cell_4;

		switch (MHC::CalculateBunchProfile(BunchIndex(), m_RefinementLevel))
		{
		case MHC::BunchProfile::C:
			bunch.NE = cell_1;
			bunch.NW = cell_2;
			bunch.SW = cell_3;
			bunch.SE = cell_4;
			break;

		case MHC::BunchProfile::D:
			bunch.SW = cell_1;
			bunch.SE = cell_2;
			bunch.NE = cell_3;
			bunch.NW = cell_4;
			break;

		case MHC::BunchProfile::U:
			bunch.NE = cell_1;
			bunch.SE = cell_2;
			bunch.SW = cell_3;
			bunch.NW = cell_4;
			break;

		case MHC::BunchProfile::N:
			bunch.SW = cell_1;
			bunch.NW = cell_2;
			bunch.NE = cell_3;
			bunch.SE = cell_4;
			break;

		default:
			break;
		}

		return bunch;
	}

	bool operator==(const CellNode& c1, const CellNode& c2)
	{
		return c1.m_GlobalIndex == c2.m_GlobalIndex;
	}

	bool operator!=(const CellNode& c1, const CellNode& c2)
	{
		return c1.m_GlobalIndex != c2.m_GlobalIndex;
	}

}