#include "algpch.h"
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

	std::string CellNode::GetMHCIndexAsBinaryString()
	{
		std::string str = std::bitset<2 * MAX_REFINEMENT_LEVEL>(m_GlobalIndex).to_string();
		for (int i = 0; i < MAX_REFINEMENT_LEVEL-1; i++)
			str.insert(3*i+2, 1, '.');
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
		for (int i = 0; i < m_RefinementLevel-1; i++)
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
		std::cout << "| Cell " << GetMHCIndexAsBinaryString();
		std::cout << std::endl;

		std::cout << "|\tCenter= (";
		std::cout << Center().x << ", " << Center().y;
		std::cout << ")";
		std::cout << std::endl;

		std::cout << "|\tRefLevel= " << (unsigned short)RefinementLevel();
		std::cout << std::endl;
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