/**
*	Modified Hilberts Curve

*/
#include "algpch.h"
#include "MHC.h"

namespace alg {
	unsigned short MHC::s_Table[4][4] = {
		{1, 0, 0, 3},
		{0, 1, 1, 2},
		{3, 2, 2, 1},
		{2, 3, 3, 0}
	};
	
	MHC::BunchProfile MHC::CalculateBunchProfile(uint32_t index, uint32_t ref_level)
	{
		uint32_t n1 = index;
		uint32_t level = ref_level;
		uint32_t i = 0;
		for (uint32_t k = 1; k < level; k++)
		{
			i = MHC::s_Table[i][n1 % 4];
			n1 = n1 / 4;
		}

		return BunchProfile(i);
	}
}