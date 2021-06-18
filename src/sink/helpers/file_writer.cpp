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
    : file_path(filename)
{ 
    file_path.make_preferred();
}

LogFile::~LogFile()
{ }
    
void LogFile::write(FileRecordData &data)
{
    if (file_stream.fail()) {
        return;
    }
    if (!file_stream.is_open() && !open_file()) {
        return;
    }
    file_stream.write(data.data.c_str(), data.data.length());
    file_stream.write("\n", 1);
    file_stream.flush();
}

bool LogFile::open_file()
{
    auto dir = file_path;
    dir.remove_filename();
    std::filesystem::create_directories(dir);

    file_stream.open(file_path, ios::out | ios::app | ios::binary);

    return !file_stream.fail();
}

std::string LogFile::get_filename() const
{
    return file_path.u8string();
}

} // namespace logging