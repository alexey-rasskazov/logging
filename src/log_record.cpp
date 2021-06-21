#include <logging/log_record.h>
#include <logging/logger.h>
#include <codecvt>
#include <locale>

namespace logging {

/** Default constructor */
LogRecord::LogRecord()
    : logger(nullptr)
    , data(LogLevel::DISABLED)
{ }

/** Parameterized Constructor */
LogRecord::LogRecord(Logger *logger, LogLevel level, const char *file_name, int line_number)
    : logger(logger)
    , data(level, file_name, line_number)
{ }

LogRecord::~LogRecord()
{
    if (data) {
        logger->write_record(&data);
    }
}

LogRecord::LogRecord(LogRecord &&src) noexcept
    : data(LogLevel::DISABLED)
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

const ILogRecordData* LogRecord::get_data() const 
{
    return &data;
}

/**
 *  Is log_level disabled
 */
bool LogRecord::is_enabled() const noexcept
{
    return data && data.log_level < LogLevel::DISABLED;
}

std::string wstring_to_utf8(const wchar_t *str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

} // namespace logger
