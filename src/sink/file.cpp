#include "file.h"
#include <vector>
#include <ctime>
#include <string.h>
#include "helpers/filename_template.h"
#include "helpers/file_writer.h"

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
    std::unique_ptr<LogFile> file;
    std::tm last_record_tm;

    Impl();
    void write_record(ILogRecordData *record, IFormatter *formatter);

};

FileSink::Impl::Impl()
{
    memset(&last_record_tm, 0, sizeof(last_record_tm));
}

void FileSink::Impl::write_record(ILogRecordData *record, IFormatter *formatter)
{
    auto data = std::make_shared<FileRecordData>();
    if (formatter) {
        formatter->format_record(data.get(), record);
    }

    data->data = record->get_data();
    std::tm record_tm = record->get_tm();
    if (!file || filename_template.is_need_rotate(record_tm, last_record_tm)) {
        file = std::make_unique<LogFile>(filename_template.generate_filename(record_tm));
    }

    file->write(data);
    last_record_tm = record_tm;
}

/*
 *
 *  FileSink class
 *
 */

FileSink::FileSink()
    : pimpl(std::make_unique<Impl>())
{ }

void FileSink::init_pimpl()
{
    pimpl = std::make_unique<Impl>();
}

FileSink::~FileSink() = default;

void FileSink::set_template(const std::string& file_template, unsigned int max_num_files)
{
    pimpl->filename_template.set_template(file_template);
}

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