#pragma once

#include <memory>
#include "../logging.h"
#include "../formatter.h"

namespace logging {

class Formatter;

/**
 * @brief Base class for log sinks.
 * 
 */
class BaseSink : public ILogSink
{
public:

    /**
     * @brief Construct a new Base Sink object
     * 
     */
    BaseSink();

    /**
     * @brief Construct a new Base Sink object
     * 
     * @tparam T The template parameter can be logging::Formatter, std::string, and char*
     * @param formatter 
     */
    template<class T>
    BaseSink(T&& formatter);

    std::string get_format() const;

    template<class T>
    void set_formatter(T&& formatter);
    
    void reseset_format();

protected:

    std::unique_ptr<Formatter> sink_formatter;
};

template<class T>
BaseSink::BaseSink(T&& formatter)
    : sink_formatter(std::make_unique<Formatter>(std::forward<T>(formatter)))
{ }

template<class T>
void BaseSink::set_formatter(T&& formatter)
{
    sink_formatter = std::make_unique<Formatter>(std::forward<T>(formatter));
}

} // namespace logging