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

    /**
     * @brief Construct a new File Sink object
     * 
     * @param file_template     
     * @param max_num_files
     */
    FileSink(const std::string &file_template, unsigned int max_num_files = 0);

    /**
     * @brief Construct a new File Sink object
     * 
     * @tparam Fmt
     * @param formatter 
     * @param file_template 
     * @param max_num_files 
     */
    template <typename Fmt>
    FileSink(Fmt &&formatter, const std::string &file_template, unsigned int max_num_files = 0);

    explicit FileSink() = delete;
    explicit FileSink(const Formatter &formatter) = delete;
    explicit FileSink(Formatter &&formatter) = delete;

    ~FileSink();

    std::string get_filename() const;

    virtual void write(ILogRecordData *record, IFormatter *logger_formatter) override;

private:

    class Impl;
    std::unique_ptr<Impl> pimpl;
};

template <typename Fmt>
FileSink::FileSink(Fmt &&formatter, const std::string& file_template, unsigned int max_num_files) 
    : FileSink(file_template, max_num_files)
{
    set_formatter(std::forward<Fmt>(formatter));
}

} // namespace logging