/**
 * logging.h - Logging interface for Paper Mario 64 PC port.
 *
 * Uses libultraship's spdlog-based logging system.
 *
 * Log files are stored at:
 *   <app_directory>/logs/PaperShip.log
 *   (rotating file sink: 10MB max per file, up to 10 backup files)
 *
 * The log directory is created automatically next to the PaperShip binary.
 *
 * Usage from C code:
 *   #include "port/logging.h"
 *   PM64_LOG_INFO("Player HP: %d", playerHP);
 *   PM64_LOG_WARN("Texture not found: %s", texName);
 *   PM64_LOG_ERROR("Failed to load map %d", mapID);
 *
 * Usage from C++ code:
 *   SPDLOG_INFO("Message {}", value);   // spdlog native macros (from spdlog/spdlog.h)
 *
 * Log levels (from most to least verbose):
 *   TRACE < DEBUG < INFO < WARN < ERROR < CRITICAL
 *
 * The default log level is DEBUG in debug builds, INFO in release builds.
 * Adjustable at runtime via CVar: gDeveloperTools.LogLevel (0-5)
 */

#ifndef PM64_LOGGING_H
#define PM64_LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

/* Log level enum matching spdlog::level */
typedef enum {
    PM64_LOG_LEVEL_TRACE = 0,
    PM64_LOG_LEVEL_DEBUG = 1,
    PM64_LOG_LEVEL_INFO = 2,
    PM64_LOG_LEVEL_WARN = 3,
    PM64_LOG_LEVEL_ERROR = 4,
    PM64_LOG_LEVEL_CRITICAL = 5,
} PM64LogLevel;

/**
 * Log a message at the given level.
 * Uses printf-style formatting.
 */
void pm64_log(PM64LogLevel level, const char* file, int line, const char* fmt, ...);

/* Convenience macros with automatic file/line info */
#define PM64_LOG_TRACE(fmt, ...) pm64_log(PM64_LOG_LEVEL_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define PM64_LOG_DEBUG(fmt, ...) pm64_log(PM64_LOG_LEVEL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define PM64_LOG_INFO(fmt, ...)  pm64_log(PM64_LOG_LEVEL_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define PM64_LOG_WARN(fmt, ...)  pm64_log(PM64_LOG_LEVEL_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define PM64_LOG_ERROR(fmt, ...) pm64_log(PM64_LOG_LEVEL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define PM64_LOG_CRITICAL(fmt, ...) pm64_log(PM64_LOG_LEVEL_CRITICAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* PM64_LOGGING_H */
