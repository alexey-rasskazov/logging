#pragma once

#include <memory>
#include "logging.h"

namespace logging {

class Formatter;

/**
 * @brief Log handler that writes to std::cout.
 * 
 */
class CoutHandler : public ILogHandler
{
public:

    CoutHandler();

    CoutHandler(const Formatter& formatter);

    CoutHandler(Formatter&& formatter);

    ~CoutHandler();

    std::string get_format() const;

    void set_format(const Formatter& formatter);

    void set_format(Formatter&& formatter);
    
    void reseset_format();
    
    virtual void write(ILogRecordData *record, IFormatter *logger_formatter) override;

private:

    std::unique_ptr<Formatter> handler_formatter;

    void write_formatted(ILogRecordData *record, IFormatter *formatter);
};

} // namespace logging