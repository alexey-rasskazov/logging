#include <logging/log_level.h>
#include <cstring>

namespace logging {

const char* log_level_name(LogLevel level)
{
    switch (level) {
    case LogLevel::DEBUG:       return "DEBUG";
    case LogLevel::INFO:        return "INFO";
    case LogLevel::WARNING:     return "WARNING";
    case LogLevel::ERROR:       return "ERROR";
    case LogLevel::FATAL:       return "FATAL";
    case LogLevel::DISABLED:    return "DISABLED";
    }
    return "";
}

LogLevel log_level_by_name(const char* name)
{
    switch (name[0]) {
    case 'D':   if (strcmp(name + 1, "EBUG") == 0) return LogLevel::DEBUG;
                if (strcmp(name + 1, "ISABLED") == 0) return LogLevel::DISABLED;
    case 'd':   if (strcmp(name + 1, "ebug") == 0) return LogLevel::DEBUG;
                if (strcmp(name + 1, "isabled") == 0) return LogLevel::DISABLED;
    break;

    case 'I':   if (strcmp(name + 1, "NFO") == 0) return LogLevel::INFO;
    case 'i':   if (strcmp(name + 1, "nfo") == 0) return LogLevel::INFO;
    break;

    case 'W':   if (strcmp(name + 1, "ARNING") == 0) return LogLevel::WARNING;
    case 'w':   if (strcmp(name + 1, "arning") == 0) return LogLevel::WARNING;
    break;

    case 'E':   if (strcmp(name + 1, "RROR") == 0) return LogLevel::ERROR;
    case 'e':   if (strcmp(name + 1, "rror") == 0) return LogLevel::ERROR;
    break;

    case 'F':   if (strcmp(name + 1, "ATAL") == 0) return LogLevel::FATAL;
    case 'f':   if (strcmp(name + 1, "atal") == 0) return LogLevel::FATAL;
    }

    return  LogLevel::UNKNOWN;
}

}
