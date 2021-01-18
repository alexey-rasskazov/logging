#include "gtest/gtest.h"
#include <logger.h>
#include <cout_handler.h>

using namespace logging;

class LoggingTest : public ::testing::Test
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

TEST_F(LoggingTest, single_text_message)
{
    Logger log;
    log.add_handler(&handler);

    log.write(LogLevel::INFO) << "Test message";

    EXPECT_EQ(fetch_output(), "Test message" + nl);
}

TEST_F(LoggingTest, complex_message)
{
    Logger log;
    log.add_handler(&handler);

    log.write(LogLevel::INFO) << "Test message " << 12345 << " " << true;

    EXPECT_EQ(fetch_output(), "Test message 12345 true" + nl);
}

TEST_F(LoggingTest, multiple_messages)
{
    Logger log;
    log.add_handler(&handler);
    std::string expected_output;

    for (int i = 0; i < 10; ++i) {
        log.write(LogLevel::INFO) << "Test message " << i;
        expected_output += "Test message " + std::to_string(i) + nl;
    }

    EXPECT_EQ(fetch_output(), expected_output);
}

TEST_F(LoggingTest, multiple_formatted_messages)
{
    Logger log;
    log.add_handler(&handler);
    handler.set_format(Formatter("${level_name} ${message}"));
    std::string expected_output;

    for (int i = 0; i < 10; ++i) {
        LogLevel level = i < 5 ? LogLevel::INFO : LogLevel::WARNING;
        log.write(level) << "Test message " << i;
        expected_output += std::string(log_level_name(level)) 
            + " Test message " + std::to_string(i) + nl;
    }

    EXPECT_EQ(fetch_output(), expected_output);
}

TEST_F(LoggingTest, formatted_messages_level)
{
    Logger log;
    log.set_log_level(LogLevel::WARNING);
    log.add_handler(&handler);
    handler.set_format(Formatter("${level_name} ${message}"));
    std::string expected_output;

    for (int i = 0; i < 10; ++i) {
        LogLevel level = i < 5 ? LogLevel::INFO : LogLevel::WARNING;
        log.write(level) << "Test message " << i;
        if (i >= 5 ) {
            expected_output += std::string(log_level_name(level)) 
                + " Test message " + std::to_string(i) + nl;
        }
    }

    EXPECT_EQ(fetch_output(), expected_output);
}