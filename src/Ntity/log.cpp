#include <Ntity/pch.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "log.h"

namespace Ntity
{
	int Log::Init(level level/* = level::trace*/)
	{
		spdlog::set_pattern("%^> %l%$ [%x %X]: %v");
		int err = SetLevel(level);
		if (err != 0) return err;

		// 

		return 0;
	}

	int Log::SetLevel(level level)
	{
		spdlog::set_level(mapLevel(level));
		return 0;
	}

	spdlog::level::level_enum Log::mapLevel(level level)
	{
		switch (level)
		{
		case trace:
			return spdlog::level::trace;
		case debug:
			return spdlog::level::debug;
		case info:
			return spdlog::level::info;
		case warn:
			return spdlog::level::warn;
		case error:
			return spdlog::level::err;
		case fatal:
			return spdlog::level::critical;
		case off:
			return spdlog::level::off;
		}
	}
}