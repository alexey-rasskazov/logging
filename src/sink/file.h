#pragma once

#include "base.h"

namespace logging {

/**
 * @brief Log sink that writes to a file.
 * 
 *  A file is defined by a filename template.
 *  The template can contain following variables:
 *  %Y - year YYYY
 *  %m - month number (01 - 12)
 *  %d - day of month (01 - 31)
 *  %j - day of year (001 - 366)
 *  %W - week of the year as a decimal number (Monday is the first day of the week) (00 - 53)
 *  %H - hour as a decimal number (00 - 23)
 * 
 *  Files are rotated automatically according to the template.
 */
class FileSink : public BaseSink
{
public:

    FileSink();

    /**
     * @brief Construct a new File Sink object
     * 
     * @tparam T The template parameter can be logging::Formatter, std::string, and char*
     * @param formatter 
     */
    template<class T>
    FileSink(T&& formatter);

    ~FileSink();

    void set_template(const std::string& file_template, unsigned int max_num_files = 0);

    std::string get_filename() const;

    virtual void write(ILogRecordData *record, IFormatter *logger_formatter) override;

private:

    class Impl;
    std::unique_ptr<Impl> pimpl;

    void init_pimpl();
};

template<class T>
FileSink::FileSink(T&& formatter)
    : BaseSink(std::forward<T>(formatter))
    , pimpl(nullptr)
{
    init_pimpl();
}

} // namespace logging