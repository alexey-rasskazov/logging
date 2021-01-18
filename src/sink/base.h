#pragma once

#include <memory>
#include "../logging.h"

namespace logging {

class Formatter;

/**
 * @brief Log handler that writes to std::cout.
 * 
 */
class BaseSink : public ILogSink
{
public:

    BaseSink();

    BaseSink(const Formatter& formatter);

    BaseSink(Formatter&& formatter);

    ~BaseSink();

    std::string get_format() const;

    void set_format(const Formatter& formatter);

    void set_format(Formatter&& formatter);
    
    void reseset_format();

protected:

    std::unique_ptr<Formatter> sink_formatter;
};

} // namespace logging