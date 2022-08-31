#pragma once
namespace alg {
	class MHC {
	public:
		enum class Profile {
			C, U, D, N
		};

		static unsigned short s_Table[4][4];

		static Profile CalculateMHCProfile(uint32_t index, uint32_t level);
	};
}

