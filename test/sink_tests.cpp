#include "gtest/gtest.h"
#include <iostream>
#include <logging/sink/cout.h>
#include <logging/log_level.h>
#include "fake_record_data.h"

using namespace logging;


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
 *  CoutSink tests
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
    
    CoutSink handler;
    
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