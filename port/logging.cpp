/**
 * logging.cpp - C-accessible logging bridge to spdlog.
 *
 * Provides printf-style logging from game C code through
 * libultraship's spdlog integration.
 */

#include "logging.h"
#include <spdlog/spdlog.h>
#include <cstdarg>
#include <cstdio>

extern "C" void pm64_log(PM64LogLevel level, const char* file, int line, const char* fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    auto loc = spdlog::source_loc{ file, line, "" };

    switch (level) {
        case PM64_LOG_LEVEL_TRACE:
            spdlog::default_logger_raw()->log(loc, spdlog::level::trace, buf);
            break;
        case PM64_LOG_LEVEL_DEBUG:
            spdlog::default_logger_raw()->log(loc, spdlog::level::debug, buf);
            break;
        case PM64_LOG_LEVEL_INFO:
            spdlog::default_logger_raw()->log(loc, spdlog::level::info, buf);
            break;
        case PM64_LOG_LEVEL_WARN:
            spdlog::default_logger_raw()->log(loc, spdlog::level::warn, buf);
            break;
        case PM64_LOG_LEVEL_ERROR:
            spdlog::default_logger_raw()->log(loc, spdlog::level::err, buf);
            break;
        case PM64_LOG_LEVEL_CRITICAL:
            spdlog::default_logger_raw()->log(loc, spdlog::level::critical, buf);
            break;
    }
}
