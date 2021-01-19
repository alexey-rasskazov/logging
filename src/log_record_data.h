#pragma once

#include <string>
#include "logging.h"

namespace logging {

/**
 *	LogRecordData class.
 */
class LogRecordData final : public ILogRecordData
{
public:

	LogRecordData(LogLevel log_level);

	LogRecordData(LogLevel log_level, const char* file_name, int line_number);

	// disable copy and move semantic

	LogRecordData(const LogRecordData& rhs) = delete;
	LogRecordData& operator = (const LogRecordData& src) = delete;
	LogRecordData(LogRecordData&& rhs) = delete;
	LogRecordData& operator = (LogRecordData&& src) = delete;


    // ILogRecordData interface

	virtual unsigned long add_ref() override;
	virtual unsigned long release() override;
	virtual const char* get_data() const override;
	virtual int64_t get_data_length(bool add_filename) const override;
	virtual LogLevel get_level() const override;
	virtual int64_t get_time() const override;
	virtual const char* get_file_name() const override;
	virtual int get_line_number() const override;

private:

	~LogRecordData() = default;

	friend class LogRecord;

    unsigned long counter;
	std::string data;
	LogLevel log_level;
	std::string file_name;
	int line_number;
	int64_t milliseconds;

};

}
