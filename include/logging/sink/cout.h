#pragma once

#include "base.h"

namespace logging {

/**
 * @brief Log sink that writes to std::cout.
 * 
 */
class CoutSink : public BaseSink
{
public:

    CoutSink() = default;
    
    template<class T>
    CoutSink(T&& formatter) : BaseSink(std::forward<T>(formatter)) {}

    virtual void write(ILogRecordData *record, IFormatter *logger_formatter) override;

private:

    void write_formatted(ILogRecordData *record, IFormatter *formatter);
};

} // namespace logging