#include "log_record_data.h"
#include <chrono>
#include "log_level.h"

namespace logging {

LogRecordData::LogRecordData(LogLevel log_level)
    : LogRecordData(log_level, "", 0)
{ }

LogRecordData::LogRecordData(LogLevel log_level, const char* file_name, int line_number)
    : counter(1)
    , log_level(log_level)
    , file_name(file_name)
    , line_number(line_number)
{ 
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    milliseconds = static_cast<int64_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
    );
}

unsigned long LogRecordData::add_ref()
{
    return ++counter;
}

unsigned long LogRecordData::release()
{
    unsigned long count = --counter;

    if (count)
        return count;

    delete this;
    return 0;
}

const char* LogRecordData::get_data()
{
    return data.c_str();
}

LogLevel LogRecordData::get_level()
{
    return log_level;
}

int64_t LogRecordData::get_time()
{
    return milliseconds;
}

const char* LogRecordData::get_file_name()
{
    return file_name.c_str();
}

int LogRecordData::get_line_number()
{
    return line_number;
}

} // namespace logger
