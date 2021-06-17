#include <logging/logger.h>
#include <algorithm>

namespace logging {

Logger::Logger() : log_level(LogLevel::DEBUG) {}

std::string Logger::get_format() const
{
    return log_formatter ? log_formatter->get_format() : "";
}

void Logger::reseset_formatter()
{
    log_formatter.reset();
}

LogRecord Logger::write(LogLevel level)
{
    if (level < log_level || level == LogLevel::DISABLED) {
        return {};
    }

    return {this, level};
}

LogRecord Logger::write(LogLevel level, const char* file_name, int line_number)
{
    if (level < log_level || level == LogLevel::DISABLED) {
        return {};
    }

    return {this, level, file_name, line_number};
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
        handler->write(record, log_formatter.get());
    }
}

} // namespace logger