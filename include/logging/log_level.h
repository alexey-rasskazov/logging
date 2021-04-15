#pragma once

namespace logging {

/**
 * @brief Logging level enumeration.
 * 
 */
enum class LogLevel
{
    DEBUG       = 10,
    INFO        = 20,
    WARNING     = 30,
    ERROR       = 40,
    FATAL       = 50,
    DISABLED    = 100,
    
    UNKNOWN     = 255,
};

const char* log_level_name(LogLevel level);

LogLevel log_level_by_name(const char* name);

} // logging
