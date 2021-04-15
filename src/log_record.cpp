#include <logging/log_record.h>
#include <logging/logger.h>
#include "log_record_data.h"

namespace logging {

/** Default constructor */
LogRecord::LogRecord():
      logger(nullptr)
    , log_level(LogLevel::DISABLED)
    , data(nullptr)
{ }

/** Parameterized Constructor */
LogRecord::LogRecord(Logger *logger, LogLevel level, const char *file_name, int line_number)
{
    this->logger = logger;
    this->log_level = level;
    if (is_enabled()) {
        data = new LogRecordData(level, file_name, line_number);
    } else {
        data = nullptr;
    }
}

/** Copy constructor */
LogRecord::LogRecord(const LogRecord &src)
{
    logger = src.logger;
    log_level = src.log_level;
    src.data->add_ref();
    data = src.data;
}

/** Destructor */
LogRecord::~LogRecord()
{
    if (data)
    {
        if (data->counter == 1)
        {
            logger->write_record(data);
        }
        data->release();
    }
}

/** Copy operator */
LogRecord& LogRecord::operator = (const LogRecord& src)
{
    if (data) {
        data->release();
    }
    logger = src.logger;
    log_level = src.log_level;
    src.data->add_ref();
    data = src.data;
    return *this;
}

/**
 *  Is log_level disabled
 */
bool LogRecord::is_enabled() const
{
    return log_level < LogLevel::DISABLED;
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
LogRecord& LogRecord::append_value_to_string(LogRecord* record, V value)
{
    if (record->is_enabled()) {
        record->data->data.append(std::to_string(value));
    }
    return *record;
}

// Append integer values

LogRecord& LogRecord::operator << (short val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (int val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (long val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (long long val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (unsigned short val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (unsigned int val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (unsigned long val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (unsigned long long val)
{
    return append_value_to_string(this, val);
}

// Append floating point values

LogRecord& LogRecord::operator << (float val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (double val)
{
    return append_value_to_string(this, val);
}

LogRecord& LogRecord::operator << (long double val)
{
    return append_value_to_string(this, val);
}

} // namespace logger
