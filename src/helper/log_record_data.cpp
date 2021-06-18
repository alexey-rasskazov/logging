#include <logging/helper/log_record_data.h>
#include <chrono>
#include <logging/log_level.h>
#include <logging/helper/datetime.h>

namespace logging {

LogRecordData::LogRecordData(LogLevel log_level)
    : LogRecordData(log_level, "", 0)
{ }

LogRecordData::LogRecordData(LogLevel log_level, const char* file_name, int line_number)
    : line_number(line_number)
    , file_name(file_name)
    , log_level(log_level)
{
    if (log_level < LogLevel::DISABLED) {
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        milliseconds = static_cast<int64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
        );
    } else {
        milliseconds = 0;
    }
    datetime.tm_year = 0;
}

LogRecordData::LogRecordData(LogRecordData &&rhs) noexcept
{
    *this = std::move(rhs);
}

LogRecordData& LogRecordData::operator = (LogRecordData &&rhs) noexcept
{
    data = std::move(rhs.data);
    log_level = rhs.log_level;
    file_name = std::move(rhs.file_name);
    line_number = rhs.line_number;
    milliseconds = rhs.milliseconds;
    datetime = rhs.datetime;
    rhs.milliseconds = 0;
    return *this;
}

const char* LogRecordData::get_data() const
{
    return data.c_str();
}

int64_t LogRecordData::get_data_length(bool add_filename) const 
{
    return add_filename ? data.length() + file_name.length() : data.length();
}

LogLevel LogRecordData::get_level() const
{
    return log_level;
}

int64_t LogRecordData::get_time() const
{
    return milliseconds;
}

std::tm LogRecordData::get_tm() const
{
    if (datetime.tm_year == 0) {
         local_datetime(&datetime, static_cast<time_t>(milliseconds/1000));
    }
    return datetime;
}

const char* LogRecordData::get_file_name() const
{
    return file_name.c_str();
}

int LogRecordData::get_line_number() const
{
    return line_number;
}

} // namespace logger
