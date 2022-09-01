#pragma once
namespace alg {
	class MHC {
	public:
		enum class Profile {
			C, U, D, N
		};
		static Profile CalculateBunchProfile(uint32_t index, uint32_t ref_level);
	
		static unsigned short s_Table[4][4];
	};
}

