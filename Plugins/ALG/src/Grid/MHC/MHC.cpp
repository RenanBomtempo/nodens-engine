#include "algpch.h"
#include "MHC.h"

namespace alg {
	unsigned short MHC::s_Table[4][4] = {
		{1, 0, 0, 3},
		{0, 1, 1, 2},
		{3, 2, 2, 1},
		{2, 3, 3, 0}
	};

	MHC::Profile MHC::CalculateMHCProfile(uint32_t index, uint32_t level)
	{
		// Calculate MHC orientation
		uint32_t n1 = index;
		uint32_t i = 0;
		for (uint32_t k = 1; k < level+1; k++)
		{
			i = MHC::s_Table[i][n1 % 4];
			n1 = n1 / 4;
		}

		return Profile(i);
	}
}