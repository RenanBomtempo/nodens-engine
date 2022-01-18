#pragma once

#include "mxpch.h"

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Moxxi {

	class MOXXI_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core Log Macros
#define MX_CORE_ERROR(...) ::Moxxi::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MX_CORE_WARN(...)  ::Moxxi::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MX_CORE_INFO(...)  ::Moxxi::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MX_CORE_TRACE(...) ::Moxxi::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MX_CORE_FATAL(...) ::Moxxi::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Core Log Macros
#define MX_ERROR(...) ::Moxxi::Log::GetClientLogger()->error(__VA_ARGS__)
#define MX_WARN(...)  ::Moxxi::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MX_INFO(...)  ::Moxxi::Log::GetClientLogger()->info(__VA_ARGS__)
#define MX_TRACE(...) ::Moxxi::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MX_FATAL(...) ::Moxxi::Log::GetClientLogger()->fatal(__VA_ARGS__)