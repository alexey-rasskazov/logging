#include "cout.h"
#include <iostream>
#include "../formatter.h"

namespace logging {


struct CoutData : public ITextData
{ 
    ~CoutData()
    {
        std::cout << std::endl;
    }

    virtual void append(const char* text) override
    {
        std::cout << text;
    }

	virtual void reserve(unsigned long size) override
    { }
};

void CoutSink::write(ILogRecordData *record, IFormatter *logger_formatter)
{
    
    if (sink_formatter) {
        write_formatted(record, sink_formatter.get());
    } else if (logger_formatter) {
        write_formatted(record, logger_formatter);
    } else {
        std::cout << record->get_data() << std::endl;
    }
}

void CoutSink::write_formatted(ILogRecordData *record, IFormatter *formatter)
{
    CoutData data;
    formatter->format_record(&data, record);
}

} // namespace logging