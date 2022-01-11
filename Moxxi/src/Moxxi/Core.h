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

