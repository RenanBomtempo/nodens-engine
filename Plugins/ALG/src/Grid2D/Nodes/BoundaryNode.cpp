#include "algpch.h"
#include "BoundaryNode.h"

namespace alg {
	NodeType BoundaryNode::m_Type = NodeType::Boundary;

	BoundaryNode::BoundaryNode()
	{
	}

	BoundaryNode::~BoundaryNode()
	{
	}
	
	void BoundaryNode::ConnectTo(TransitionNode* transition_node)
	{
		m_TransitionNode = transition_node;
		m_TransitionNode->m_Lower = this;
	}
}