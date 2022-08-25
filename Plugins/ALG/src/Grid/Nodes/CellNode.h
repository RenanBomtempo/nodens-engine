#pragma once
#include "Node.h"
#include "Math\Vector2D.h"

namespace alg {
	class CellNode : public Node
	{
		friend class Grid2D;
		friend class CellBunch;
	public:
		CellNode();
		~CellNode();

		virtual const NodeType GetType() const override { return m_Type; }

		// Printing
		std::string GetMHCIndexAsBinaryString();
		void Print();

		// Neighbors
		Node* North() { return m_North; }
		Node* South() { return m_South; }
		Node* West() { return m_West; }
		Node* East() { return m_East; }

		// Values
		const Vector2D& Center() { return m_Center; }
		const float SideLength() { return 1.0f / (1 << (m_RefLevel)); }
		uint8_t RefinementLevel() { return m_RefLevel; }

		// MHC Ordering
		uint32_t Index() { return m_MHCIndex; }
		bool HasNext() { return m_MHCNext != nullptr; }
		bool HasPrevious() { return m_MHCPrevious != nullptr; }
		CellNode* Next();
		CellNode* Previous();

		friend bool operator==(const CellNode& c1, const CellNode& c2);
		friend bool operator!=(const CellNode& c1, const CellNode& c2);

	private:
		void Disconnect(Node* other);

	private:
		// Neighbors 
		Node* m_North;
		Node* m_East;
		Node* m_South;
		Node* m_West;

		// Cell's center coordinates
		Vector2D m_Center;

		// Modified Hilbert's Curve (MHC) Ordering
		CellNode* m_MHCNext;
		CellNode* m_MHCPrevious;
		uint32_t m_MHCIndex;

		uint8_t m_RefLevel;

	private:
		static NodeType m_Type;
	};
}
