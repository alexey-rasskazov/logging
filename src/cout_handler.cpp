#include "cout_handler.h"
#include <iostream>
#include "formatter.h"

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

CoutHandler::CoutHandler()
{ }

CoutHandler::CoutHandler(const Formatter& formatter)
    : handler_formatter(std::make_unique<Formatter>(formatter))
{ }

CoutHandler::CoutHandler(Formatter&& formatter)
    : handler_formatter(std::make_unique<Formatter>(std::move(formatter)))
{ }

CoutHandler::~CoutHandler()
{ }

std::string CoutHandler::get_format() const
{
    return handler_formatter ? handler_formatter->get_format() : "";
}

void CoutHandler::set_format(const Formatter& format)
{
    handler_formatter = std::make_unique<Formatter>(format);
}

void CoutHandler::set_format(Formatter&& format)
{
    handler_formatter = std::make_unique<Formatter>(std::move(format));
}

void CoutHandler::reseset_format()
{
    handler_formatter.reset();
}

void CoutHandler::write(ILogRecordData *record, IFormatter *logger_formatter)
{
    
    if (handler_formatter) {
        write_formatted(record, handler_formatter.get());
    } else if (logger_formatter) {
        write_formatted(record, logger_formatter);
    } else {
        std::cout << record->get_data() << std::endl;
    }
}

void CoutHandler::write_formatted(ILogRecordData *record, IFormatter *formatter)
{
    CoutData data;
    formatter->format_record(&data, record);
}

} // namespace logging