#include "file_writer.h"
#include <iostream>

using namespace std;

namespace logging {

void FileRecordData::append(const char* text)
{
    data.append(text);
}

void FileRecordData::reserve(unsigned long size)
{
    data.reserve(size);
}

LogFile::LogFile(const std::string& filename)
    : file_name(filename)
{ }

LogFile::~LogFile()
{ }
    
void LogFile::write(FileRecordData &data)
{
    if (file_stream.fail()) {
        return;
    }
    if (!file_stream.is_open()) {
        file_stream.open(file_name.c_str(), ios::out | ios::app | ios::binary);
        if (file_stream.fail()) {
            return;
        }
    }
    file_stream.write(data.data.c_str(), data.data.length());
    file_stream.write("\n", 1);
    file_stream.flush();
}

std::string LogFile::get_filename() const
{
    return file_name;
}

} // namespace logging