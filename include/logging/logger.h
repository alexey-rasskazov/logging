#pragma once

#include <vector>
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
    Logger();

    /**
     * @brief Construct a new Logger object
     * 
     * @tparam T    The template parameter can be logging::Formatter, std::string, and char*
     * @param formatter 
     */
    template<class T>
    Logger(T&& formatter);

    /**
     * @brief Destroy the Logger object
     * 
     */
    ~Logger() = default;

    std::string get_format() const;

    template<class T>
    void set_formatter(T&& formatter);

    void reseset_formatter();

    LogRecord write(LogLevel level);

    LogRecord write(LogLevel level, const char* file_name, int line_number);

    void set_log_level(LogLevel level);

    LogLevel get_log_level() const;

    bool add_handler(ILogSink* handler);

    void remove_handler(ILogSink* handler);

private:

    friend class LogRecord;

    void write_record(ILogRecordData* record);

    LogLevel log_level;

    std::unique_ptr<Formatter> log_formatter;

    std::vector<ILogSink*> handlers;
};

template<class T>
Logger::Logger(T&& formatter)
    : log_formatter(std::make_unique<Formatter>(std::forward<T>(formatter)))
    , log_level(LogLevel::DEBUG)
{ }

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