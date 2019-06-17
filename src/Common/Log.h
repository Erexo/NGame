#pragma once

#include "spdlog/spdlog.h"

class Log
{
public:
	enum level
	{
		trace,
		debug,
		info,
		warn,
		error,
		fatal,
		off
	};

	static int Init(level level = level::trace);
	static int SetLevel(level level);

private:
	static spdlog::level::level_enum mapLevel(level level);
};

#define N_TRACE(...)	spdlog::trace(__VA_ARGS__)
#define N_DEBUG(...)	spdlog::debug(__VA_ARGS__)
#define N_INFO(...)		spdlog::info(__VA_ARGS__)
#define N_WARN(...)		spdlog::warn(__VA_ARGS__)
#define N_ERROR(...)	spdlog::error(__VA_ARGS__)
#define N_FATAL(...)	spdlog::critical(__VA_ARGS__)