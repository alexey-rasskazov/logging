#include <logging/log_record.h>
#include <logging/logger.h>
#include "log_record_data.h"

namespace logging {

/** Default constructor */
LogRecord::LogRecord()
    : logger(nullptr)
{ }

/** Parameterized Constructor */
LogRecord::LogRecord(Logger *logger, LogLevel level, const char *file_name, int line_number)
    : logger(logger)
    , data(level, file_name, line_number)
{ }

LogRecord::~LogRecord()
{
    if (data) {
        logger->write_record(data.get());
    }
}

LogRecord::LogRecord(LogRecord &&src) noexcept
{
    logger = src.logger;
    data = std::move(src.data);
}

LogRecord& LogRecord::operator = (LogRecord &&src) noexcept
{
    logger = src.logger;
    data = std::move(src.data);
    return *this;
}

ILogRecordData* LogRecord::get_data() const 
{
    return data.get();
}

/**
 *  Is log_level disabled
 */
bool LogRecord::is_enabled() const
{
    return data && data->log_level < LogLevel::DISABLED;
}

/**
 * Append string
 */
LogRecord& LogRecord::operator << (const std::string &val)
{
    if (is_enabled()) {
        data->data.append(val);
    }
    return *this;
}

/**
 * Append char array
 */
LogRecord& LogRecord::operator << (const char* val)
{
    if (is_enabled()) {
        data->data.append(val);
    }
    return *this;
}

/**
 * Append bool ("true" or "false" literal)
 */
LogRecord& LogRecord::operator << (bool val)
{
    if (is_enabled()) {
        data->data.append(val ? "true" : "false");
    }
    return *this;
}

template<class V>
LogRecord& LogRecord::append_value_to_string(V value)
{
    if (is_enabled()) {
        data->data.append(std::to_string(value));
    }
    return *this;
}

// Append integer values

LogRecord& LogRecord::operator << (short val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (int val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (long val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (long long val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (unsigned short val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (unsigned int val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (unsigned long val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (unsigned long long val)
{
    return append_value_to_string(val);
}

// Append floating point values

LogRecord& LogRecord::operator << (float val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (double val)
{
    return append_value_to_string(val);
}

LogRecord& LogRecord::operator << (long double val)
{
    return append_value_to_string(val);
}

} // namespace logger
