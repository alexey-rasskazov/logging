#include <logging/logger.h>

/*
 * Fake RecordData object.
 */
struct FakeRecordData : public logging::ILogRecordData
{
    std::string data;
    logging::LogLevel log_level;
    std::string file_name;
    int line_number;
    int64_t milliseconds;

    FakeRecordData(
        logging::LogLevel level = logging::LogLevel::INFO,
        const char* text = "",
        const char* file = "",
        int line = 0,
        int64_t ms = 1610462801012
    )
        : log_level(level)
        , data(text)
        , file_name(file)
        , line_number(line)
        , milliseconds(ms)
    {}

    // ILogRecordData interface
    virtual const char* get_data() const override;
    virtual int64_t get_data_length(bool add_filename) const override;
    virtual logging::LogLevel get_level() const override;
    virtual int64_t get_time() const override;
    virtual const char* get_file_name() const override;
    virtual int get_line_number() const override;
};