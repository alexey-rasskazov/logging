#pragma once

#include <string>
#include <logging/logging.h>

namespace logging {

/**
 *    LogRecordData class.
 */
class LogRecordData final : public ILogRecordData
{
public:

    LogRecordData(LogLevel log_level);
    LogRecordData(LogLevel log_level, const char* file_name, int line_number);

    LogRecordData(LogRecordData &&src) noexcept;
    LogRecordData& operator = (LogRecordData &&src) noexcept;

    // disable copy semantic

    LogRecordData(const LogRecordData&) = delete;
    LogRecordData& operator = (const LogRecordData&) = delete;

    explicit operator bool() const noexcept { return milliseconds != 0; }

    // ILogRecordData interface

    virtual const char* get_data() const override;
    virtual int64_t get_data_length(bool add_filename) const override;
    virtual LogLevel get_level() const override;
    virtual int64_t get_time() const override;
    virtual std::tm get_tm() const override;
    virtual const char* get_file_name() const override;
    virtual int get_line_number() const override;

private:

    friend class LogRecord;

    int64_t milliseconds;
    int line_number;
    std::string file_name;
    std::string data;
    mutable std::tm datetime;
    LogLevel log_level;
};

}
