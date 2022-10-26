#pragma once
#include "Grid2D/Nodes/Node.h"
#include "Grid2D/Direction.h"
#include "Math/Vector2D.h"

namespace alg {
	class CellNode : public Node
	{
		friend class Grid2D;
		friend struct CellBunch;
	public:
		CellNode();
		~CellNode();

		virtual const NodeType GetType() const override { return m_Type; }

		// Printing ------------------------------------------------------------
		std::string BunchIndexAsBinaryString();
		std::string GlobalIndexAsBinaryString();
		void Print();

		// Neighbors -----------------------------------------------------------
		Node* North() { return m_North; }
		Node* South() { return m_South; }
		Node* West() { return m_West; }
		Node* East() { return m_East; }

		// Values
		const Vector2D& Center() { return m_Center; }
		const float SideLength() { return 1.0f / (1 << (m_RefinementLevel)); }
		uint8_t RefinementLevel() { return m_RefinementLevel; }

		// MHC Ordering --------------------------------------------------------
		
		/*The Global Index identifies the cell within the complete MHC ordering.

		  It is composed of 2*MAX_REFINEMENT_LEVEL bits where the last 
		  (2 * (m_RefinementLevel-1)) bits are the bunch's index, and the 
		  remaining 2 bits represent the cell's index within the bunch.
		  
		  Ex: Refinement Level = 5
		  Global index = 00.00.10.11.01.10.00 
		  Bunch  index = --.--.--.11.01.10.00  =>  00.00.00.11.01.10.00
		  Local  index = --.--.10.--.--.--.--  =>  00.00.00.00.00.00.10
		*/
		uint32_t GlobalIndex();

		/* Returns the index of the bunch which the cell is a part of.
		*/
		uint32_t GlobalBunchIndex();

		uint32_t LocalBunchIndex();

		/* Returns the index of the cell within its bunch.
		*/
		uint32_t LocalIndex();

		bool HasNext();
		bool HasPrevious();
		CellNode* Next();
		CellNode* Previous();
		
		Node* GetNeighbor(Direction direction);
		
		/* Identify cells from the same bunch in the MHC order
		*/
		CellBunch GetBunch();

		friend bool operator==(const CellNode& c1, const CellNode& c2);
		friend bool operator!=(const CellNode& c1, const CellNode& c2);

	private:
		void Disconnect(Node* other);
		void Connect(Direction direction, Node* neighbor);

	private:
		// Neighbors 
		Node* m_North;
		Node* m_East;
		Node* m_South;
		Node* m_West;

		// Cell's center coordinates
		Vector2D m_Center;

		// Modified Hilbert's Curve (MHC) Ordering
		uint32_t m_GlobalIndex;
		CellNode* m_Next;
		CellNode* m_Previous;

		uint8_t m_RefinementLevel;

	private:
		static NodeType m_Type;
	};
}
