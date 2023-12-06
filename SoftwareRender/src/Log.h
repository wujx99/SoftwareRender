#pragma once



#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

#include <memory>

namespace SR {
	class  Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}

// Core log macros
#define SR_TRACE(...)  ::SR::Log::GetLogger()->trace(__VA_ARGS__)
#define SR_INFO(...)  ::SR::Log::GetLogger()->info(__VA_ARGS__)
#define SR_WARN(...)  ::SR::Log::GetLogger()->warn(__VA_ARGS__)
#define SR_ERROR(...)  ::SR::Log::GetLogger()->error(__VA_ARGS__)
#define SR_CRITICAL(...)  ::SR::Log::GetLogger()->critical(__VA_ARGS__)


// if distribute build 下面重写上面的宏保证对应二进制文件不会包含上面的宏！