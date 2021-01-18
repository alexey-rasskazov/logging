#pragma once

#include <vector>
#include <memory>
#include "logging.h"
#include "log_record.h"
#include "formatter.h"

namespace logging {

/**
 *	Logger class.
 */
class  Logger
{
public:
	
	/** Constructor. */
	Logger();

	/** Destructor. */
	~Logger();

	LogRecord write(LogLevel level);

	void set_log_level(LogLevel level);

	LogLevel get_log_level() const;

	bool add_handler(ILogHandler* handler);

	void remove_handler(ILogHandler* handler);

private:

	friend class LogRecord;

    void write_record(ILogRecordData* record);

	LogLevel log_level;

	std::vector<ILogHandler*> handlers;
};

}
