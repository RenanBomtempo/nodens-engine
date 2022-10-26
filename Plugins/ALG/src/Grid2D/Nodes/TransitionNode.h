#pragma once
#include "Node.h"
#include "Log.h"

namespace alg {
	class TransitionNode : public Node
	{
		friend class Grid2D;
		friend class BoundaryNode;
	public:
		TransitionNode();
		~TransitionNode();

		inline virtual const NodeType GetType() const override { return m_Type; };
		void Disconnect(Node* other);

	private:
		// Connections
		Node* m_Lower; // Lower refinement level Node.
		Node* m_HigherNorW; // Up/Left higher refinement level Node.
		Node* m_HigherSorE; // Down/Right higher refinement level Node.

		static NodeType m_Type;
	};
}
