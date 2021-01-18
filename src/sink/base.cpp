#include "base.h"
#include "../formatter.h"

namespace logging {

BaseSink::BaseSink()
{ }

BaseSink::BaseSink(const Formatter& formatter)
    : sink_formatter(std::make_unique<Formatter>(formatter))
{ }

BaseSink::BaseSink(Formatter&& formatter)
    : sink_formatter(std::make_unique<Formatter>(std::move(formatter)))
{ }

BaseSink::~BaseSink()
{ }

std::string BaseSink::get_format() const
{
    return sink_formatter ? sink_formatter->get_format() : "";
}

void BaseSink::set_format(const Formatter& format)
{
    sink_formatter = std::make_unique<Formatter>(format);
}

void BaseSink::set_format(Formatter&& format)
{
    sink_formatter = std::make_unique<Formatter>(std::move(format));
}

void BaseSink::reseset_format()
{
    sink_formatter.reset();
}

} // namespace logging