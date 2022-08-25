#pragma once
#include "Core.h"

namespace alg {
	enum class NodeType {
		Transition=0, Cell, Boundary
	};

	class Node
	{
	public:
		Node();
		~Node();

		virtual const NodeType GetType() const = 0;
	};
}
