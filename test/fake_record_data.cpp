#include "fake_record_data.h"

const char* FakeRecordData::get_data() const
{
    return data.c_str();
}

int64_t FakeRecordData::get_data_length(bool add_filename) const
{
    return add_filename ? data.length() + file_name.length() : data.length();
}

logging::LogLevel FakeRecordData::get_level() const
{
    return log_level;
}

int64_t FakeRecordData::get_time() const
{
    return milliseconds;
}

std::tm FakeRecordData::get_tm() const
{
    time_t t = static_cast<time_t>(milliseconds / 1000); return *localtime(&t);
}

const char* FakeRecordData::get_file_name() const
{
    return file_name.c_str();
}

int FakeRecordData::get_line_number() const
{
    return line_number;
}