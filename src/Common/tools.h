#pragma once

#ifndef LOG_ERROR
#ifdef __ERROR_LOG__
#include <iostream>
#define LOG_ERROR(msg) { std::cout << "[" << __FUNCTION__ << "] Error: " << msg << std::endl; }
#else
#define LOG_ERROR(msg) { }
#endif
#endif

#ifndef LOG_TRACE
#ifdef __TRACE_LOG__
#include <iostream>
#define LOG_TRACE(msg) { std::cout << "[" << __FUNCTION__ << "] Trace: " << msg << std::endl; }
#else
#define LOG_TRACE(msg) { }
#endif
#endif