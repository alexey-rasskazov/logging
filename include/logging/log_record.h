#pragma once

#include "log_level.h"
#include <string>

namespace logging {

class Logger;
class LogRecordData;

/**
 *    LogRecord class.
 */
class LogRecord
{
public:

    /** Copy constructor */
    LogRecord(const  LogRecord &src);

    /** Destructor */
    ~LogRecord();

    /** Copy operator */
    LogRecord& operator = (const LogRecord& src);


    // TODO add move semantic

    /*
     *    Left shift operators to append data to the record.
     */

    // Append string

    LogRecord& operator << (const std::string &val);

    // Append char array

    LogRecord& operator << (const char* val);

    // Append bool ("true" or "false" literal)

    LogRecord& operator << (bool val);

    // Append integer values

    LogRecord& operator << (short val);
    LogRecord& operator << (int val);
    LogRecord& operator << (long val);
    LogRecord& operator << (long long val);
    LogRecord& operator << (unsigned short val);
    LogRecord& operator << (unsigned int val);
    LogRecord& operator << (unsigned long val);
    LogRecord& operator << (unsigned long long val);

    // Append floating point values

    LogRecord& operator << (float val);
    LogRecord& operator << (double val);
    LogRecord& operator << (long double val);


    /**
     * @brief Member template for custom objects
     *        T must be converted to std::string 
     * 
     * @tparam T 
     * @param val 
     * @return LogRecord& 
     */
    template<typename T>
    LogRecord& operator << (const T& val);

protected:

    LogRecordData* data;

private:

    friend class Logger;

    template<class V>
    static LogRecord& append_value_to_string(LogRecord* record, V value);

    LogRecord();

    LogRecord(Logger *logger, LogLevel level, const char *file_name = "", int line_number = 0);

    bool is_enabled() const;

    void append_str(const std::string& str);

    Logger* logger;
    LogLevel log_level;
};

template<typename T>
LogRecord& LogRecord::operator << (const T& val)
{
    if (is_enabled()) {
        append_str(static_cast<std::string>(val));
    }
    return *this;
}

}