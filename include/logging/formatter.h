#pragma once

#include <string>
#include <memory>
#include "logging.h"

namespace logging {

/**
 *  @brief Formatter of a log record.
 * 
 *  The template of record format can contain following variables:
 *  ${time[:format]} - timestamp of record creation,
 *  ${level} - number of log level of record,
 *  ${level_name} - name of log level of record,
 *  ${file} - file name,
 *  ${line} - line number,
 *  ${message} - record message text.
 * 
 *  The time is formatted according to the "strftime" function,
 *  but you can also use "%f" in the time format to output milliseconds value.
 *  Time format is %Y-%m-%d %H:%M:%S.%f by default.
 */    
class Formatter : public IFormatter
{
public:

    Formatter(const std::string& format = "${time:%Y-%m-%d %H:%M:%S.%f} [${level_name}] ${message}");

    ~Formatter();

    Formatter(const Formatter& rhs);

    Formatter& operator = (const Formatter& rhs);

    Formatter(Formatter&& rhs) noexcept;

    Formatter& operator = (Formatter&& rhs) noexcept;

    void set_format(const std::string& format);

    std::string get_format() const;

    std::string format_record(ILogRecordData *data);

    virtual void format_record(ITextData *result, ILogRecordData *record, ITimeFormatter *time_fmt = nullptr) override;

private:

    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

}