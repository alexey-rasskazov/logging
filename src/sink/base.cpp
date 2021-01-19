#include "base.h"

namespace logging {

BaseSink::BaseSink()
    : sink_formatter(nullptr)
{ }

std::string BaseSink::get_format() const
{
    return sink_formatter ? sink_formatter->get_format() : "";
}

void BaseSink::reseset_formatter()
{
    sink_formatter.reset();
}

} // namespace logging