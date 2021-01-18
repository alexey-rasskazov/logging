#include "logger.h"
#include <algorithm>

namespace logging {

/** Constructor. */
Logger::Logger() :
    log_level(LogLevel::DEBUG)
{

}

/** Destructor. */
Logger::~Logger()
{

}

LogRecord Logger::write(LogLevel level)
{
    if (level < log_level || level == LogLevel::DISABLED) {
		return LogRecord();
    }

    return LogRecord(this, level);
}

void Logger::set_log_level(LogLevel level) 
{
    log_level = level;
}

LogLevel Logger::get_log_level() const 
{
    return log_level;
}

bool Logger::add_handler(ILogSink* handler)
{
    if (std::find(handlers.begin(), handlers.end(), handler) == handlers.end()) {
        handlers.push_back(handler);
        return true;
    }
    return false;
}

void Logger::remove_handler(ILogSink* handler)
{
    handlers.erase(
        std::remove(handlers.begin(), handlers.end(), handler),
        handlers.end()
    );
}

void Logger::write_record(ILogRecordData* record)
{
    for (auto handler : handlers) {
        handler->write(record, nullptr);
    }
}

} // namespace logger