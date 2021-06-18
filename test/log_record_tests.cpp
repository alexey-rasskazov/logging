#include "gtest/gtest.h"
#include <logging/logger.h>
#include <logging/helper/log_record_data.h>

using namespace logging;

/*
 * LogRecord for testing
 */
class TestingLogRecord : public LogRecord
{
public:

    TestingLogRecord(LogRecord &&src) noexcept : LogRecord(std::move(src)) {}

    const ILogRecordData* get_data() const
    {
        return LogRecord::get_data();
    }
};

class LogRecordTest : public ::testing::Test
{
protected:

    void SetUp() override {}
    
    Logger log;

    int64_t time_delay;
    LogLevel log_level;
    std::string record_text;
    std::string file_name;
    int line_number;

    void fetch_record_data(TestingLogRecord& rec)
    {
        auto data = rec.get_data();
        time_delay = int64_t(time(nullptr)) - data->get_time()/1000;
        log_level = data->get_level();
        record_text = data->get_data();
        file_name = data->get_file_name();
        line_number = data->get_line_number();
    }
};

TEST_F(LogRecordTest, empty)
{
    TestingLogRecord rec = log.write(LogLevel::WARNING);

    fetch_record_data(rec);
    EXPECT_LE(time_delay, 1);
    EXPECT_EQ(log_level, LogLevel::WARNING);
    EXPECT_EQ(record_text, "");
    EXPECT_EQ(file_name, "");
    EXPECT_EQ(line_number, 0);
}

TEST_F(LogRecordTest, append_text_literal)
{
    TestingLogRecord rec = std::move(log.write(LogLevel::WARNING) << "test message");

    fetch_record_data(rec);
    EXPECT_LE(time_delay, 1);
    EXPECT_EQ(log_level, LogLevel::WARNING);
    EXPECT_EQ(record_text, "test message");
    EXPECT_EQ(file_name, "");
    EXPECT_EQ(line_number, 0);
}

TEST_F(LogRecordTest, append_string)
{
    std::string message = "test message";
    TestingLogRecord rec = std::move(log.write(LogLevel::WARNING) << message);

    fetch_record_data(rec);
    EXPECT_LE(time_delay, 1);
    EXPECT_EQ(log_level, LogLevel::WARNING);
    EXPECT_EQ(record_text, message);
    EXPECT_EQ(file_name, "");
    EXPECT_EQ(line_number, 0);
}

TEST_F(LogRecordTest, append_bools)
{
    bool val = false;
    TestingLogRecord rec = std::move(log.write(LogLevel::INFO) << val << " " << true);

    fetch_record_data(rec);
    EXPECT_LE(time_delay, 1);
    EXPECT_EQ(log_level, LogLevel::INFO);
    EXPECT_EQ(record_text, "false true");
    EXPECT_EQ(file_name, "");
    EXPECT_EQ(line_number, 0);
}

TEST_F(LogRecordTest, append_int)
{
    int number = 123;
    TestingLogRecord rec = std::move(log.write(LogLevel::INFO) << number);

    fetch_record_data(rec);
    EXPECT_LE(time_delay, 1);
    EXPECT_EQ(log_level, LogLevel::INFO);
    EXPECT_EQ(record_text, std::to_string(number));
    EXPECT_EQ(file_name, "");
    EXPECT_EQ(line_number, 0);
}

TEST_F(LogRecordTest, append_long_long)
{
    long long number = -1234567891011;
    TestingLogRecord rec = std::move(log.write(LogLevel::INFO) << number);

    fetch_record_data(rec);
    EXPECT_LE(time_delay, 1);
    EXPECT_EQ(log_level, LogLevel::INFO);
    EXPECT_EQ(record_text, std::to_string(number));
    EXPECT_EQ(file_name, "");
    EXPECT_EQ(line_number, 0);
}

TEST_F(LogRecordTest, append_mixed_numbers)
{
    int int_number = 123;
    long long ll_number = -1234567891011;
    short short_number = 12345;
    TestingLogRecord rec = std::move(log.write(LogLevel::INFO)
        << int_number << ll_number 
        << " " << short_number);

    std::string expected_text = std::to_string(int_number) 
        + std::to_string(ll_number) 
        + " " + std::to_string(short_number);

    fetch_record_data(rec);
    EXPECT_LE(time_delay, 1);
    EXPECT_EQ(log_level, LogLevel::INFO);
    EXPECT_EQ(record_text, expected_text);
    EXPECT_EQ(file_name, "");
    EXPECT_EQ(line_number, 0);
}

TEST_F(LogRecordTest, append_mixed_all)
{
    int int_number = 123;
    long long ll_number = -1234567891011;
    short short_number = 12345;
    bool b_val = true;
    std::string message = "test";
    float flt_val = 0.5;
    double dbl_val = 1.5;
    long double ldbl_val = 2.e40;
    TestingLogRecord rec = std::move(log.write(LogLevel::INFO) 
        << int_number << ll_number << " " << short_number
        << " " << b_val  << " " << message 
        << " " << flt_val << " " << dbl_val << " " << ldbl_val);

    std::string expected_text = 
        std::to_string(int_number) + std::to_string(ll_number) 
        + " " + std::to_string(short_number)
        + " true " + message 
        + " " + std::to_string(flt_val) 
        + " " + std::to_string(dbl_val) 
        + " " + std::to_string(ldbl_val);

    fetch_record_data(rec);
    EXPECT_LE(time_delay, 1);
    EXPECT_EQ(log_level, LogLevel::INFO);
    EXPECT_EQ(record_text, expected_text);
    EXPECT_EQ(file_name, "");
    EXPECT_EQ(line_number, 0);
}