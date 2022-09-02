#include "algpch.h"
#include "CellBunch.h"

namespace alg {
	CellBunch::CellBunch(Vector2D center, uint32_t refinement_level)
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
		double center_offset = 1.0 / (double)((uint8_t)1 << (refinement_level + 1));

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
}