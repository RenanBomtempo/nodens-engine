#pragma once
#include <cstdint>

#ifdef ALG_DEBUG
	#define ALG_ENABLE_ASSERTS
#endif

#ifdef ALG_ENABLE_ASSERTS
	#define ALG_ASSERT(x, ...) { if(!(x)) { std::cout << "Assertion Failed:" << __VA_ARGS__; __debugbreak(); } }
	#define ALG_CORE_ASSERT(x, ...) { if(!(x)) { std::cout << "Assertion Failed:" << __VA_ARGS__; __debugbreak(); } }
#else
	#define ALG_ASSERT(x, ...)
	#define ALG_CORE_ASSERT(x, ...)
#endif // ALG_ENABLE_ASSERTS

#define MAX_REFINEMENT_LEVEL 8
