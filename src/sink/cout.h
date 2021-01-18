#pragma once

#include "base.h"

namespace logging {

class Formatter;

/**
 * @brief Log handler that writes to std::cout.
 * 
 */
class CoutSink : public BaseSink
{
public:

    virtual void write(ILogRecordData *record, IFormatter *logger_formatter) override;

private:

    void write_formatted(ILogRecordData *record, IFormatter *formatter);
};

} // namespace logging