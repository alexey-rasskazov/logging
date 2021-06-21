#pragma once

#include "log_level.h"
#include "logging.h"
#include "helper/log_record_data.h"
#include <string>
#include <sstream>
#include <type_traits>

namespace logging {

class Logger;

/**
 *    LogRecord class.
 */
class LogRecord
{
public:

    ~LogRecord();

    LogRecord(const LogRecord&) = delete;
    LogRecord& operator = (const LogRecord&) = delete;

    LogRecord(LogRecord &&src) noexcept;
    LogRecord& operator = (LogRecord &&src) noexcept;

    /**
     * @brief Left shift operator to append data to the record, writes specified object as a string.
     * 
     * @tparam T Must be intergral types, floating point types, char*, std::string or converted to std::string 
     * @param val 
     * @return LogRecord& 
     */
    template<typename T>
    LogRecord& operator << (T &&val);

protected:

    const ILogRecordData* get_data() const;

private:

    friend class Logger;

    LogRecord();
    LogRecord(Logger *logger, LogLevel level, const char *file_name = "", int line_number = 0);

    bool is_enabled() const noexcept;

    inline void append_str(std::string &&value)
    {
        data.data.append(std::move(value));
    }

    static std::string wstring_to_utf8(const wchar_t *str);

    Logger* logger;
    LogRecordData data;
};

template<typename T>
LogRecord& LogRecord::operator << (T &&val)
{
    if (is_enabled()) {
        using underlying_array_type = 
            std::remove_cv_t<
                std::remove_pointer_t<
                    std::decay_t<T>
                >
            >;
        using underlying_type =
            std::remove_cv_t<
                std::remove_reference_t<T>
            >;
        if constexpr (std::is_same_v<underlying_type, char>) {
            data.data += val;
        }
        else if constexpr (std::is_same_v<underlying_type, bool>) {
            data.data.append(val ? "true" : "false");
        }
        else if constexpr (
                   std::is_integral_v<underlying_type> 
                || std::is_floating_point_v<underlying_type>) {
            append_str(std::to_string(val));
        }
        else if constexpr (std::is_same_v<underlying_array_type, char>) {
            data.data.append(val);
        }
        else if constexpr (std::is_same_v<underlying_array_type, wchar_t>) {
            append_str(wstring_to_utf8(val));
        }
        else if constexpr (
                   std::is_same_v<underlying_type, std::string>
                || std::is_same_v<underlying_type, std::string_view>) {
            data.data.append(std::forward<T>(val));
        }
        else if constexpr (
                   std::is_same_v<underlying_type, std::wstring>
                || std::is_same_v<underlying_type, std::wstring_view>) {
            append_str(wstring_to_utf8(val.c_str()));
        }
        else if constexpr (std::is_convertible_v<underlying_type, std::string>) {
            append_str(static_cast<std::string>(val));
        }
        else {
            std::ostringstream ss;
            ss << val;
            data.data.append(ss.str());
        }
    }
    return *this;
}

}