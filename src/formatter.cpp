#include <logging/formatter.h>
#include <stdio.h>
#include <vector>
#include <numeric>
#include <cstring>
#include <logging/log_level.h>

namespace logging {

struct TextData : public ITextData
{
    std::string data;
    
    virtual void append(const char* text) override
    {
        data.append(text);
    }

    virtual void reserve(unsigned long size) override
    {
        data.reserve(size);
    }
};

/**
 * @brief Detects if format has milliseconds part (%f sequence)
 * 
 * @param format 
 * @return true 
 * @return false 
 */
bool is_format_has_milliseconds(const std::string& format)
{
    std::size_t i = 0;
    std::size_t len = format.length();

    while (i < len - 1) {
        if (format[i] == '%') {
            if (format[i + 1] == '%') {
                ++i;
            } else if (format[i + 1] == 'f') {
                return true;
            }
        }
        ++i;
    }
    return false;
}

/**
 * @brief Helper function, prepares %f milliseconds time format.
 * 
 * @param format 
 * @return true 
 * @return false 
 */
bool prepare_time_format(std::string& format)
{
    bool has_ms = is_format_has_milliseconds(format);
 
    if (has_ms) {
        size_t i = 0;
        size_t len = format.length();
        while (i < len - 1) {
            if (format[i] == '%') {
                if (format[i + 1] == '%') {
                    format.replace(i, 2, "%%%%");
                    i += 3;
                    len = format.length();
                } else if (format[i + 1] == 'f') {
                    format.replace(i, 2, "%%03u");
                    i += 4;
                    len = format.length();
                }
            }
            ++i;
        }
    }

    return has_ms;
}

/**
 * @brief Applies date and time format
 * 
 * @param target 
 * @param format 
 * @param datetime 
 * @param milliseconds 
 */
void format_date_and_time(ITextData *target, const std::string& format, std::tm& datetime, int64_t milliseconds)
{
    char time_str[40];

    std::strftime(time_str, sizeof(time_str), format.c_str(), &datetime);
    if (milliseconds) {
        char time_str_ms[40];
        snprintf(time_str_ms, sizeof(time_str_ms), time_str, milliseconds % 1000);
        target->append(time_str_ms);
    } else {
        target->append(time_str);
    }
}

std::size_t calc_formatted_time_length(const std::string& format, bool is_millisecons)
{
    TextData data;
    int64_t ms = 1610462801012;
    std::tm datetime;
    std::memset(&datetime, 0, sizeof(datetime));
    datetime.tm_sec = 41;
    datetime.tm_min = 46;
    datetime.tm_hour = 17;
    datetime.tm_mday = 12;
    datetime.tm_mon = 0;
    datetime.tm_year = 121;
    datetime.tm_wday = 2;
    datetime.tm_yday = 11;
    datetime.tm_isdst = 0;
    
    format_date_and_time(&data, format, datetime, is_millisecons ? ms : 0);
    return data.data.length();
}

/**
 * @brief Type of format unit of formatter.
 * 
 */
enum class FormatUnitType
{
    TIME,
    LEVEL,
    LEVEL_NAME,
    FILE,
    LINE,
    TEXT,
    MESSAGE,
};

/**
 * @brief Format unit.
 * 
 */
struct FormatUnit 
{
    FormatUnitType type;
    std::string text;
    bool is_msec;
    
    FormatUnit(
        FormatUnitType type,
        const std::string& text = "",
        bool is_msec = false)
        : type(type)
        , text(text)
        , is_msec(is_msec)
    { }

    std::size_t get_length() const
    {
        switch (type) {
            case FormatUnitType::TIME:          return calc_formatted_time_length(text, is_msec);
            case FormatUnitType::LEVEL:         return 3;
            case FormatUnitType::LEVEL_NAME:    return 8;
            case FormatUnitType::FILE:          return 0;
            case FormatUnitType::LINE:          return 4;
            case FormatUnitType::TEXT:          return text.length();
        }
        return 0;
    }
};

void format_record_date_and_time(ITextData* target, const FormatUnit& unit, ILogRecordData* record)
{
    std::tm datetime = record->get_tm();

    format_date_and_time(
        target,
        unit.text,
        datetime, 
        unit.is_msec ? record->get_time() : 0
    );
}

/**
 * @brief Formatter implementation
 * 
 */
struct Formatter::Impl
{
    std::string format_str;
    std::vector<FormatUnit> format_units;
    std::size_t format_length;
    bool has_file;

    /**
     * @brief Set the format string
     * 
     * @param format 
     */
    void set_format(const std::string& format)
    {
        using std::string;

        std::size_t i = 0, start = 0;
        std::size_t len = format.length();
        bool is_message = false;
        has_file = false;

        format_units.clear();

        while (i < len) {
            if (format[i] == '$' && i + 1 < len && format[i + 1] == '{') {
                if (i > start) {
                    format_units.emplace_back(FormatUnitType::TEXT, format.substr(start, i - start));
                }
                start = i + 2;
                i = format.find_first_of('}', i + 2);
                if (i == string::npos) {
                    i = start = len;
                } else {
                    is_message = add_format_unit(format.substr(start, i - start)) || is_message;
                    i++;
                    start = i;
                }
            }
            i++;
        }

        if (i > start) {
            format_units.emplace_back(FormatUnitType::TEXT, format.substr(start, i - start));
        }

        if (!is_message) {
            format_units.emplace_back(FormatUnitType::MESSAGE);
        }

        format_str = format;

        format_length = std::accumulate(
            format_units.begin(), format_units.end(), size_t{0},
            [](size_t a, const FormatUnit& b) { return a + b.get_length(); }
        );
    }

    /**
     * @brief Adds format element to format_units
     * 
     * @param element 
     * @return true 
     * @return false 
     */
    bool add_format_unit(std::string element)
    {
        size_t fmt_pos = element.find_first_of(':');
        std::string element_fmt;
        if (fmt_pos != std::string::npos) {
            element_fmt = element.substr(fmt_pos + 1);
            element = element.substr(0, fmt_pos);
        }
        if (element == "time") {
            bool has_ms = prepare_time_format(element_fmt);
            format_units.emplace_back(FormatUnitType::TIME, element_fmt, has_ms);
        } else if (element == "level_name") {
            format_units.emplace_back(FormatUnitType::LEVEL_NAME);
        } else if (element == "level") {
            format_units.emplace_back(FormatUnitType::LEVEL);
        } else if (element == "file") {
            format_units.emplace_back(FormatUnitType::FILE);
            has_file = true;
        } else if (element == "line") {
            format_units.emplace_back(FormatUnitType::LINE);
        } else if (element == "message") {
            format_units.emplace_back(FormatUnitType::MESSAGE);
            return true;
        }
        return false;
    }

    /**
     * @brief Calculates record size
     * 
     * @param record 
     * @return std::size_t 
     */
    std::size_t calc_record_size(ILogRecordData *record)
    {
        return format_length + record->get_data_length(has_file);
    }
};

/*
 *  Formatter class
 */

/** Constructor */
Formatter::Formatter(const std::string& format)
    : pimpl(std::make_unique<Impl>())
{
    set_format(format);
}

Formatter::~Formatter() = default;

Formatter::Formatter(const Formatter& rhs)
    : pimpl(nullptr)
{
    if (rhs.pimpl) {
        pimpl = std::make_unique<Impl>(*rhs.pimpl);
    }
}

Formatter& Formatter::operator = (const Formatter& rhs)
{
    if (!rhs.pimpl) {
        pimpl.reset();
    } else if (!pimpl) {
        pimpl = std::make_unique<Impl>(*rhs.pimpl);
    } else {
        *pimpl = *rhs.pimpl;
    }
    return *this;
}

Formatter::Formatter(Formatter&& rhs) noexcept = default;

Formatter& Formatter::operator = (Formatter&& rhs) noexcept = default;

void Formatter::set_format(const std::string& format)
{
    if (pimpl) {
        pimpl->set_format(format);
    }
}

std::string Formatter::format_record(ILogRecordData* record)
{
    TextData result;

    format_record(&result, record);
    
    return result.data;
}

void Formatter::format_record(ITextData *result, ILogRecordData *record)
{
    if (!pimpl) {
        result->append(record->get_data());
        return;
    }

    result->reserve(static_cast<unsigned long>(pimpl->calc_record_size(record)));

    for (const auto& unit : pimpl->format_units)
    {
        switch (unit.type)
        {
        case FormatUnitType::TEXT:
            result->append(unit.text.c_str());
            break;

        case FormatUnitType::TIME:
            format_record_date_and_time(result, unit, record);
            break;
                
        case FormatUnitType::LEVEL:
            result->append(std::to_string(static_cast<int>(record->get_level())).c_str());
            break;

        case FormatUnitType::LEVEL_NAME:
            result->append(log_level_name(record->get_level()));
            break;
                
        case FormatUnitType::FILE:
            result->append(record->get_file_name());
            break;
                
        case FormatUnitType::LINE:
            result->append(std::to_string(record->get_line_number()).c_str());
            break;

        case FormatUnitType::MESSAGE:
            result->append(record->get_data());   
            break;
        }
    }
}

std::string Formatter::get_format() const
{
    if (pimpl) {
        return pimpl->format_str;
    } else {
        return "";
    }
}

} // namespace logging
