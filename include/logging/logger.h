#pragma once

#include <memory>
#include "logging.h"
#include "log_record.h"
#include "formatter.h"

namespace logging {

/**
 *    Logger class.
 */
class  Logger
{
public:
    
    /**
     * @brief Construct a new Logger object
     * 
     */
    Logger(LogLevel level = LogLevel::DEBUG);

    /**
     * @brief Construct a new Logger object
     * 
     * @tparam T    The template parameter can be logging::Formatter, std::string, and char*
     * @param formatter 
     */
    template<class T>
    Logger(T&& formatter, LogLevel level = LogLevel::DEBUG);

    /**
     * @brief Destroy the Logger object
     * 
     */
    ~Logger();

    std::string get_format() const;

    template<class T>
    void set_formatter(T&& formatter);

    void reseset_formatter();

    LogRecord write(LogLevel level);

    LogRecord write(LogLevel level, const char* file_name, int line_number);

    void set_level(LogLevel level);

    LogLevel get_level() const;

    bool add_sink(ILogSink* sink);

    void remove_sink(ILogSink* sink);

private:

    friend class LogRecord;

    void write_record(ILogRecordData* record);

    class Impl;

    std::unique_ptr<Impl> pimpl;
    LogLevel log_level;
    std::unique_ptr<Formatter> log_formatter;
};

template<class T>
Logger::Logger(T&& formatter, LogLevel level) : Logger(level)
{
    log_formatter = std::make_unique<Formatter>(std::forward<T>(formatter));
}

template<class T>
void Logger::set_formatter(T&& formatter)
{
    log_formatter = std::make_unique<Formatter>(std::forward<T>(formatter));
}

}

#ifdef LOG_FILE_LINE
#  define WRITE_LOG(log, level) (log).write((level), __FILE__, __LINE__)
#else
#  define WRITE_LOG(log, level) (log).write(level)
#endif