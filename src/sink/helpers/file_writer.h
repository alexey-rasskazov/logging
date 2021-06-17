#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <logging/logging.h>

namespace logging {

struct FileRecordData : public ITextData
{
    std::string data;
    virtual void append(const char* text) override;
    virtual void reserve(unsigned long size) override;
};

class LogFile
{
public:
    
    LogFile(const std::string& filename);

    virtual ~LogFile();
    
    virtual void write(FileRecordData &data);

    std::string get_filename() const;
    
private:

    std::string file_name;
    std::fstream file_stream;
};


} // namespace logging