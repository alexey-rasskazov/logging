#include <logging/logger.h>
#include <vector>
#include <algorithm>

namespace logging {

class Logger::Impl
{
public:

    bool add_sink(ILogSink* sink)
    {
        if (std::find(sinks.begin(), sinks.end(), sink) == sinks.end()) {
            sinks.push_back(sink);
            return true;
        }
        return false;
    }

    void remove_sink(ILogSink* sink)
    {
        sinks.erase(
            std::remove(sinks.begin(), sinks.end(), sink),
            sinks.end()
        );
    }

    void write_record(ILogRecordData* record, Formatter *formatter)
    {
        for (auto sink : sinks) {
            sink->write(record, formatter);
        }
    }

private:
    std::vector<ILogSink*> sinks;
};

Logger::Logger(LogLevel level)
    : pimpl{std::make_unique<Impl>()}
    , log_level(level)
{ }


Logger::~Logger() = default;

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

void Logger::set_level(LogLevel level) 
{
    log_level = level;
}

LogLevel Logger::get_level() const 
{
    return log_level;
}

bool Logger::add_sink(ILogSink* sink)
{
    return pimpl->add_sink(sink);
}

void Logger::remove_sink(ILogSink* sink)
{
    pimpl->remove_sink(sink);
}

void Logger::write_record(ILogRecordData* record)
{
    pimpl->write_record(record, log_formatter.get());
}

} // namespace logger