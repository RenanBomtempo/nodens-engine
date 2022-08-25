#pragma once

#include "algpch.h"

#include "Core.h"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace alg {
	/*
	Static logger used by the entire library. 
	*/
	class Log
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
#define ALG_CORE_ERROR(...) ::alg::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ALG_CORE_WARN(...)  ::alg::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ALG_CORE_INFO(...)  ::alg::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ALG_CORE_TRACE(...) ::alg::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ALG_CORE_FATAL(...) ::alg::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// USer Log Macros
#define ALG_ERROR(...) ::alg::Log::GetClientLogger()->error(__VA_ARGS__)
#define ALG_WARN(...)  ::alg::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ALG_INFO(...)  ::alg::Log::GetClientLogger()->info(__VA_ARGS__)
#define ALG_TRACE(...) ::alg::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ALG_FATAL(...) ::alg::Log::GetClientLogger()->fatal(__VA_ARGS__)