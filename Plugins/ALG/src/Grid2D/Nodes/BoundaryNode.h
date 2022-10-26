#pragma once
#include "Node.h"
#include "TransitionNode.h"


namespace alg {
	class BoundaryNode : public Node
	{
	public:
		BoundaryNode();
		~BoundaryNode();

		inline virtual const NodeType GetType() const override { return m_Type; };

		void ConnectTo(TransitionNode* transition_node);

	public:
		TransitionNode* m_TransitionNode;

	private:
		static NodeType m_Type;
	};
}

