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
    file_stream.open(file_path, ios::out | ios::app | ios::binary);

    if (file_stream.fail()) {
        std::cerr 
            << "Can't open old log file: "
            << file_path
            << std::endl;
    }
}

LogFile::~LogFile()
{ }
    
void LogFile::write(FileRecordData &data)
{
    if (file_stream.fail()) {
        return;
    }

    file_stream.write(data.data.c_str(), data.data.length());
    file_stream.write("\n", 1);
    file_stream.flush();
}

std::string LogFile::get_filename() const
{
    return file_path.u8string();
}

} // namespace logging