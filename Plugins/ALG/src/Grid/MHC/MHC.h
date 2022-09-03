#pragma once
namespace alg {
	class MHC {
	public:
		enum class BunchProfile {
			C,	//   .____.     .____.
				//	 | NW | <== | NE |
				//   |____|	    |____|
				//     ||
				//     \/
				//   .____.     .____.
				//	 | SW | ==> | SE |
				//   |____|	    |____|
			

			U,  //   .____.     .____.
				//	 | NW |     | NE |
				//   |____|	    |____|
				//	   /\         ||
				//	   ||         \/
				//   .____.     .____.
				//	 | SW | <== | SE |
				//   |____|	    |____|	


			D,	//   .____.     .____.
				//	 | NW | <== | NE |
				//   |____|	    |____|
				//                /\
				//                ||
				//   .____.     .____.
				//	 | SW | ==> | SE |
				//   |____|	    |____|

			
			N	//   .____.     .____.
				//	 | NW | ==> | NE |
				//   |____|	    |____|
				//     /\         ||
				//     ||         \/
				//   .____.     .____.
				//	 | SW |     | SE |
				//   |____|	    |____|
		};
		static BunchProfile CalculateBunchProfile(uint32_t index, uint32_t ref_level);
	
		static unsigned short s_Table[4][4];
	};
}

