#pragma once

#include <memory>

#ifdef MX_PLATFORM_WINDOWS
#if MX_DYNAMIC_LINK
	#ifdef MX_BUILD_DLL
		#define MOXXI_API __declspec(dllexport)
	#else
		#define MOXXI_API __declspec(dllimport)
	#endif // MX_BUILD_DLL
#else
	#define MOXXI_API
#endif // MX_DYNAMIC_LINK
#else
	#error Moxxi only supports windows!
#endif // MX_PLATFORM_WINDOWS

#ifdef MX_DEBUG
	#define MX_ENABLE_ASSERTS
#endif

#ifdef MX_ENABLE_ASSERTS
	#define MX_ASSERT(x, ...) { if(!(x)) { MX_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MX_CORE_ASSERT(x, ...) { if(!(x)) { MX_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MX_ASSERT(x, ...)
	#define MX_CORE_ASSERT(x, ...)
#endif // MX_ENABLE_ASSERTS

#define BIT(x) (1 << x)	// Shift 1 to the left by x places (ex 1 << 2 = 100)

#define MX_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

//std::bind(Application::OnEvent*, Application*, std::placeholders::_1)

namespace Moxxi {
	template<typename T>
	using Scope = std::unique_ptr<T>;
	
	template<typename T>
	using Ref = std::shared_ptr<T>;
	
}