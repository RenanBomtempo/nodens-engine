#pragma once

#ifdef MX_PLATFORM_WINDOWS
	#ifdef MX_BUILD_DLL
		#define MOXXI_API __declspec(dllexport)
	#else
		#define MOXXI_API __declspec(dllimport)
	#endif // MX_BUILD_DLL
#else
	#error Moxxi only supports windows!
#endif // MX_PLATFORM_WINDOWS

#ifdef MX_ENABLE_ASSERTS
	#define MX_ASSERT(x, ...) { if(!(x)) { MX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MX_CORE_ASSERT(x, ...) { if(!(x)) { MX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MX_ASSERT(x, ...)
	#define MX_CORE_ASSERT(x, ...)
#endif // MX_ENABLE_ASSERTS

#define BIT(x) (1 << x)	// Shift 1 to the left by x places (ex 1 << 2 = 100)
