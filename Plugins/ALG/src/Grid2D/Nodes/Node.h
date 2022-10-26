#pragma once
#include "Core.h"

namespace alg {
	class Node
	{
	public:
		Node();
		~Node();

		virtual const NodeType GetType() const = 0;
	};
}
