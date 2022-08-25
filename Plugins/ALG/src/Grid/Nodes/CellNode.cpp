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
		m_MHCNext(nullptr),
		m_MHCPrevious(nullptr),
		m_RefLevel(-1),
		m_Center(Vector2D(0)),
		m_MHCIndex(-1)
	{
	}

	CellNode::~CellNode()
	{
	}

	std::string CellNode::GetMHCIndexAsBinaryString()
	{
		return std::bitset<2 * MAX_REFINEMENT_LEVEL>(m_MHCIndex).to_string();
	}

	CellNode* CellNode::Next()
	{
		return m_MHCNext;
	}

	CellNode* CellNode::Previous()
	{
		return m_MHCPrevious;
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
		return c1.m_MHCIndex == c2.m_MHCIndex;
	}

	bool operator!=(const CellNode& c1, const CellNode& c2)
	{
		return c1.m_MHCIndex != c2.m_MHCIndex;
	}

}