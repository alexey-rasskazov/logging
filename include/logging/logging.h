#pragma once

#include <cstddef>
#include <cstdint>

namespace logging {

enum class LogLevel;

/**
 * @brief Log record interface
 * 
 */
struct ILogRecordData
{
    virtual const char* get_data() const = 0;
    virtual int64_t get_data_length(bool add_filename) const = 0;
    virtual LogLevel get_level() const = 0;
    virtual int64_t get_time() const = 0;
    virtual const char* get_file_name() const = 0;
    virtual int get_line_number() const = 0;
};

/**
 * @brief Text data interface
 * 
 */
struct ITextData
{
    virtual void append(const char* text) = 0;
    virtual void reserve(unsigned long size) = 0;
};

/**
 * @brief Date and time formatter interface
 * 
 */
struct ITimeFormatter
{
    virtual void format_time(char *res, size_t maxsize, const char *fmt) = 0;
};

/**
 * @brief Record data fromatter interface
 * 
 */
struct IFormatter
{
    virtual void format_record(ITextData *result, ILogRecordData *record, ITimeFormatter *time_fmt = nullptr) = 0;
};

/**
 * @brief Log sink interface
 * 
 */
struct ILogSink
{
    virtual void write(ILogRecordData *record, IFormatter *logger_formatter) = 0;
};

}