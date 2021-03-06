#include <logging/sink/file.h>
#include <vector>
#include <ctime>
#include <limits>
#include <iostream>
#include <string.h>
#include <logging/helper/datetime.h>
#include "helpers/filename_template.h"
#include "helpers/file_writer.h"
#include "helpers/convert_str.h"

namespace logging {

/*
 *
 *  FileSink::Impl class
 *
 */

class FileSink::Impl
{
public:

    FilenameTemplate filename_template;
    unsigned int max_num_files;
    std::unique_ptr<LogFile> file;
    std::tm last_record_tm;

    Impl(const std::string& file_template, unsigned int max_files);
    void write_record(ILogRecordData *record, IFormatter *formatter);
    void remove_old_files(const std::filesystem::path &dir);

    struct TimeFormatter : public ITimeFormatter
    {
        std::tm datetime;
        TimeFormatter(ILogRecordData *record)
        {
            local_datetime(&datetime, static_cast<time_t>(record->get_time()/1000));
        }

        virtual void format_time(char *res, size_t maxsize, const char *fmt) override
        {
            std::strftime(res, maxsize, fmt, &datetime);
        }
    };
};

FileSink::Impl::Impl(const std::string& file_template, unsigned int max_files)
    : filename_template{file_template}
    , max_num_files{max_files}
{
    memset(&last_record_tm, 0, sizeof(last_record_tm));
}

void FileSink::Impl::write_record(ILogRecordData *record, IFormatter *formatter)
{
    FileRecordData data;
    TimeFormatter tf{record};
    
    if (formatter) {
        formatter->format_record(&data, record, &tf);
    } else {
        data.data = record->get_data();
    }

    if (!file || filename_template.is_need_rotate(tf.datetime, last_record_tm)) {
        auto filename = filename_template.generate_filename(tf.datetime);
        std::filesystem::path dir{filename};
        dir.remove_filename();
        std::filesystem::create_directories(dir);
        file = std::make_unique<LogFile>(filename);
        remove_old_files(dir);
    }

    file->write(data);
    
    last_record_tm = tf.datetime;
}

void FileSink::Impl::remove_old_files(const std::filesystem::path &dir)
{
    if (!max_num_files || !filename_template.rotatable()) {
        return;
    }

    std::filesystem::path oldest_file_path;
    TemplateFileParams oldest_file_params;
    oldest_file_params.year = std::numeric_limits<decltype(oldest_file_params.year)>::max();
    size_t count = 0;
    
    for (auto& p: std::filesystem::directory_iterator(dir)) {
        if (p.is_regular_file()) {
            auto name = convert_str<std::string>(p.path().filename().u8string());
            auto params = filename_template.parse_filename(name);
            if (params.has_value()) {
                if (params.value() < oldest_file_params) {
                    oldest_file_params = params.value();
                    oldest_file_path = p.path();
                }
                ++count;
            }
        }
    }

    if (count > max_num_files) {
        std::error_code code;
        if (!std::filesystem::remove(oldest_file_path, code)) {
            std::cerr 
                << "Can't remove old log file: " 
                << oldest_file_path 
                << ", code: " << code
                << std::endl;
        }
    }
}

/*
 *
 *  FileSink class
 *
 */

FileSink::FileSink(const std::string& file_template, unsigned int max_num_files)
    : pimpl(std::make_unique<Impl>(file_template, max_num_files))
{ }

FileSink::~FileSink() = default;

std::string FileSink::get_filename() const
{
    return pimpl->file ? pimpl->file->get_filename() : "";
}

void FileSink::write(ILogRecordData *record, IFormatter *logger_formatter)
{
    pimpl->write_record(
        record,
        sink_formatter ? static_cast<IFormatter*>(sink_formatter.get()) : logger_formatter
    );
}

} // namespace logging