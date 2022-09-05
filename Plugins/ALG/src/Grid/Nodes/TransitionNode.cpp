#include "algpch.h"
#include "Grid/Direction.h"
#include "TransitionNode.h"

namespace alg {
	NodeType TransitionNode::m_Type = NodeType::Transition;

	TransitionNode::TransitionNode()
		:	m_Lower(nullptr),
			m_HigherNorW(nullptr),
			m_HigherSorE(nullptr)
	{
	}
	

	void TransitionNode::Disconnect(Node* other)
	{
		if (m_Lower == other) m_Lower = nullptr;
		if (m_HigherNorW == other) m_HigherNorW = nullptr;
		if (m_HigherSorE == other) m_HigherSorE = nullptr;
	}
	
	TransitionNode::~TransitionNode()
	{
	}
}