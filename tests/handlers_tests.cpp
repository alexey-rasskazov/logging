#include "gtest/gtest.h"
#include <iostream>
#include <cout_handler.h>
#include <log_level.h>

using namespace logging;

/*
 * Fake RecordData object.
 */
struct FakeRecordData : public ILogRecordData
{
    std::string data;
    LogLevel log_level;
    std::string file_name;
    int line_number;
    int64_t milliseconds;

    FakeRecordData(
        LogLevel level = LogLevel::INFO,
        const char* text = "",
        const char* file = "",
        int line = 0,
        int64_t ms = 1610462801012
    )
        : log_level(level)
        , data(text)
        , file_name(file)
        , line_number(line)
        , milliseconds(ms)
    {}

    // ILogRecordData interface

	virtual unsigned long add_ref() override { return 1; }
	virtual unsigned long release() override { return 0; }
	virtual const char* get_data() override { return data.c_str(); }
	virtual LogLevel get_level() override { return log_level; }
	virtual int64_t get_time() override { return milliseconds; }
	virtual const char* get_file_name() override { return file_name.c_str(); }
	virtual int get_line_number() override { return line_number; }
};

/* ITextData implementation */
struct TextData : public ITextData
{
    std::string data;

	virtual void append(const char* text) override
    {
        data.append(text);
    }

	virtual void reserve(unsigned long size) override
    {
        data.reserve(size);
    }
};

/* FakeFormatter */
struct FakeFormatter : public IFormatter
{
    std::string prefix;

    FakeFormatter(const std::string& prefix = "PREFIX ") : prefix(prefix) { }

    virtual void format_record(ITextData *result, ILogRecordData *record) override
    {
        result->append(prefix.c_str());
        result->append(record->get_data());
    }    
};

/*
 *
 *  CoutHandler tests
 * 
 */

class CoutHandlerTest : public ::testing::Test
{
protected:

    void SetUp() override 
    {
        external_buf = std::cout.rdbuf();
        // redirect cout to our buffer
        std::cout.rdbuf(buffer.rdbuf());
        // get std::endl as a string
        std::cout << std::endl;
        nl = buffer.str();
        // clear buffer data
        buffer.str(std::string());
    }

    void TearDown() override
    {
        // restore old buffer
        std::cout.rdbuf(external_buf);
    }
    
    CoutHandler handler;
    
    std::string fetch_output()
    {
        return buffer.str();
    }

    std::string nl;

private:
    
    std::stringstream buffer;
    std::streambuf *external_buf;
};

TEST_F(CoutHandlerTest, empty)
{
    EXPECT_EQ(fetch_output(), "");
}

TEST_F(CoutHandlerTest, single_message)
{
    FakeRecordData rec{LogLevel::INFO, "test"};
    std::string expected_output = rec.data + nl;
    
    handler.write(&rec, nullptr);
    
    EXPECT_EQ(fetch_output(), expected_output);
}

TEST_F(CoutHandlerTest, multiple_messages)
{
    FakeRecordData rec1{LogLevel::INFO, "message 1"};
    FakeRecordData rec2{LogLevel::INFO, "message 2"};
    FakeRecordData rec3{LogLevel::INFO, "message 3"};
    std::string expected_output = rec1.data + nl + rec2.data + nl + rec3.data + nl;
    
    handler.write(&rec1, nullptr);
    handler.write(&rec2, nullptr);
    handler.write(&rec3, nullptr);

    EXPECT_EQ(fetch_output(), expected_output);
}

TEST_F(CoutHandlerTest, logger_formatter)
{
    FakeFormatter fmt("TEST_FORMAT ");
    FakeRecordData rec1{LogLevel::INFO, "message 1"};
    TextData data;
    fmt.format_record(&data, &rec1);
    std::string expected_output = data.data + nl;
    
    handler.write(&rec1, &fmt);

    EXPECT_EQ(fetch_output(), expected_output);
}