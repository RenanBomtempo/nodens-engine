#include "algpch.h"
#include "Grid2D/Nodes/CellBunch.h"

namespace alg {
	CellBunch::CellBunch()
		:	NE(nullptr),
			NW(nullptr),
			SW(nullptr),
			SE(nullptr),
			first(nullptr),
			last(nullptr)
	{
		// Empty constructor
	}


	void CellBunch::Initialize(Vector2D center, uint8_t refinement_level)
	{
		NE = new CellNode();
		NW = new CellNode();
		SW = new CellNode();
		SE = new CellNode();

		NE->m_RefinementLevel = refinement_level;
		NW->m_RefinementLevel = refinement_level;
		SW->m_RefinementLevel = refinement_level;
		SE->m_RefinementLevel = refinement_level;

		// Calculate the offset between the bunch center and the center of the 
		// new cells.
		float center_offset = 1.0 / (float)(1 << (refinement_level + 1));

		// * NE
		// * *
		NE->m_South = SE;
		NE->m_West = NW;
		NE->m_Center = Vector2D(
			center.x + center_offset,
			center.y + center_offset);

		// NW *
		//  * * 
		NW->m_South = SW;
		NW->m_East = NE;
		NW->m_Center = Vector2D(
			center.x - center_offset,
			center.y + center_offset);

		//  * *
		// SW *
		SW->m_North = NW;
		SW->m_East = SE;
		SW->m_Center = Vector2D(
			center.x - center_offset,
			center.y - center_offset);

		// * *
		// * SE
		SE->m_North = NE;
		SE->m_West = SW;
		SE->m_Center = Vector2D(
			center.x + center_offset,
			center.y - center_offset);
	}

	void CellBunch::Destroy() {
		delete(NE);
		delete(NW);
		delete(SE);
		delete(SW);
	}

	Vector2D CellBunch::CalculateCenter() {
		return Vector2D(
			(NW->m_Center.x + NE->m_Center.x) / 2,
			(NW->m_Center.y + SW->m_Center.y) / 2);
	}

	void CellBunch::ConnectToAnotherBunch(Direction direction, CellBunch& other_bunch)
	{
		switch (direction)
		{
		case alg::Direction::North:
			NW->Connect(direction, other_bunch.SW);
			NE->Connect(direction, other_bunch.SE);
			break;

		case alg::Direction::East:
			NE->Connect(direction, other_bunch.NW);
			SE->Connect(direction, other_bunch.SW);
			break;

		case alg::Direction::South:
			SE->Connect(direction, other_bunch.NE);
			SW->Connect(direction, other_bunch.NW);
			break;

		case alg::Direction::West:
			NW->Connect(direction, other_bunch.NE);
			SW->Connect(direction, other_bunch.SE);
			break;

		default:
			break;
		}
	}
	
	void CellBunch::ConnectToTransition(Direction direction, TransitionNode* transition_node)
	{
		switch (direction)
		{
		case alg::Direction::North:
			NW->Connect(direction, transition_node);
			NE->Connect(direction, transition_node);
			break;

		case alg::Direction::East:
			NE->Connect(direction, transition_node);
			SE->Connect(direction, transition_node);
			break;

		case alg::Direction::South:
			SE->Connect(direction, transition_node);
			SW->Connect(direction, transition_node);
			break;

		case alg::Direction::West:
			NW->Connect(direction, transition_node);
			SW->Connect(direction, transition_node);
			break;

		default:
			break;
		}
	}
}