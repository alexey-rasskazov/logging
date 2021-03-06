#include "gtest/gtest.h"
#include <logging/formatter.h>
#include <logging/log_level.h>
#include "fake_record_data.h"
#include <logging/helper/datetime.h>

using namespace logging;

/**
 * @brief Helper function to call std::strftime
 * 
 * @param format 
 * @param millisecodns 
 * @return std::string 
 */
std::string format_datatime(const char *format, int64_t millisecodns)
{
    char time_str[40];
    time_t t = millisecodns / 1000;
    std::tm dt;
    local_datetime(&dt, t);
    std::strftime(time_str, sizeof(time_str), format, &dt);
    return time_str;
}

std::vector<std::string> explode(const std::string &text, char sep = ' ')
{
    std::vector<std::string> res;
    std::string part;
    for (auto c : text) {
        if (c == sep) {
            res.push_back(part);
            part.clear();
        } else {
            part += c;
        }
    }
    if (!part.empty() || text.back() == sep) {
        res.push_back(part);
    }
    return res;
}

/**
 * @brief Helper function to convert milliseconds to a string with leading zeros
 * 
 * @param millisecodns 
 * @return std::string 
 */
std::string milliseconds_str(int64_t milliseconds)
{
    char ms_str[4];
    snprintf(ms_str, sizeof(ms_str), "%03u", static_cast<unsigned int>(milliseconds % 1000));
    return ms_str;
}

TEST(LogFormatterTest, empty)
{
    Formatter fmt("");
    FakeRecordData rec;

    EXPECT_EQ(fmt.format_record(&rec), "");
}

TEST(LogFormatterTest, empty_fmt)
{
    Formatter fmt("");
    FakeRecordData rec{LogLevel::INFO, "test"};

    EXPECT_EQ(fmt.format_record(&rec), "test");
}

TEST(LogFormatterTest, format_msg)
{
    Formatter fmt("${time:%Y-%m-%d %H:%M:%S.%f} [${level_name}] ${message}");
    FakeRecordData rec{LogLevel::INFO, "test"};
    
    std::string line = fmt.format_record(&rec);

    EXPECT_EQ(line.substr(19), ".012 [INFO] test");
}
TEST(LogFormatterTest, format_empty_msg)
{
    Formatter fmt("${time:%Y-%m-%d %H:%M:%S.%f} [${level_name}] ${message}");
    FakeRecordData rec{LogLevel::INFO, "test"};
    
    std::string line = fmt.format_record(&rec);

    EXPECT_EQ(line.substr(19), ".012 [INFO] test");
}

TEST(LogFormatterTest, format_msg_file_line)
{
    Formatter fmt("${time:%Y-%m-%d %H:%M:%S.%f} [${level_name}] ${message}");
    FakeRecordData rec{LogLevel::INFO, "", "test.cpp", 134};
    
    std::string line = fmt.format_record(&rec);

    EXPECT_EQ(line.substr(19), ".012 [INFO] ");
}

TEST(LogFormatterTest, format_escape_char)
{
    Formatter fmt1{"$"}, fmt2{"${"};
    FakeRecordData rec{LogLevel::INFO, "test"};
    
    std::string line1 = fmt1.format_record(&rec);
    std::string line2 = fmt2.format_record(&rec);
 
    EXPECT_EQ(line1, "$test");
    EXPECT_EQ(line2, "test");
}

TEST(LogFormatterTest, format_escape_chars)
{
    Formatter fmt("$ [${level_name}] $ {} ${file} ${line}: ${message} $${}{abc} $12 $");
    FakeRecordData rec{LogLevel::INFO, "test", "test.cpp", 134};
    
    std::string line = fmt.format_record(&rec);
 
    EXPECT_EQ(line, "$ [INFO] $ {} test.cpp 134: test ${abc} $12 $");
}

TEST(LogFormatterTest, wrong_format_token)
{
    Formatter fmt("${asdf} [${level_name}] ${file} ${line}: ${message} ${{}}");
    FakeRecordData rec{LogLevel::INFO, "test", "test.cpp", 134};

    std::string line = fmt.format_record(&rec);

    EXPECT_EQ(line, " [INFO] test.cpp 134: test }");
}

TEST(LogFormatterTest, wrong_format_sequence)
{
    Formatter fmt("${123");
    FakeRecordData rec{LogLevel::INFO, "test"};

    std::string line = fmt.format_record(&rec);

    EXPECT_EQ(line, "test");
}

TEST(LogFormatterTest, format_time)
{
    std::string time_fmt_str = "%H:%M:%S";
    Formatter fmt("${time:"+ time_fmt_str +"}");
    FakeRecordData rec;
    
    std::string line = fmt.format_record(&rec);
    std::string expected_str = format_datatime(time_fmt_str.c_str(), rec.get_time());

    EXPECT_EQ(line, expected_str);
}

TEST(LogFormatterTest, format_time_ms)
{
    std::string time_fmt_str = "%H:%M:%S.";
    Formatter fmt("${time:"+ time_fmt_str +"%f}");
    FakeRecordData rec;

    std::string line = fmt.format_record(&rec); 
    std::string expected_str = format_datatime(time_fmt_str.c_str(), rec.get_time()) + milliseconds_str(rec.get_time());

    EXPECT_EQ(line, expected_str);
}

TEST(LogFormatterTest, format_data_time_ms)
{
    std::string time_fmt_str = "%y-%m-%d %a %H:%M:%S.";
    Formatter fmt("${time:"+ time_fmt_str +"%f}");
    FakeRecordData rec;
    
    std::string line = fmt.format_record(&rec); 
    std::string expected_str = format_datatime(time_fmt_str.c_str(), rec.get_time()) + milliseconds_str(rec.get_time());

    EXPECT_EQ(line, expected_str);
}

TEST(LogFormatterTest, format_default_time)
{
    Formatter fmt("${time}");
    FakeRecordData rec;

    std::string line = fmt.format_record(&rec); 
    std::string expected_str = format_datatime("%H:%M:%S", rec.get_time());

    EXPECT_EQ(line, expected_str);
}


TEST(LogFormatterTest, format_default_time_colon)
{
    Formatter fmt("${time:}");
    FakeRecordData rec;

    std::string line = fmt.format_record(&rec); 
    std::string expected_str = format_datatime("%H:%M:%S", rec.get_time());

    EXPECT_EQ(line, expected_str);
}

TEST(LogFormatterTest, format_wrong_time_format1)
{
    Formatter fmt("${time:%}");
    FakeRecordData rec;

    std::string line = fmt.format_record(&rec); 
    std::string expected_str = format_datatime("%H:%M:%S", rec.get_time());

    EXPECT_EQ(line, expected_str);
}

TEST(LogFormatterTest, format_wrong_time_format2)
{
    Formatter fmt("${time:%H%q}");
    FakeRecordData rec;

    std::string line = fmt.format_record(&rec); 
    std::string expected_str = format_datatime("%Hq", rec.get_time());

    EXPECT_EQ(line, expected_str);
}

TEST(LogFormatterTest, format_wrong_time_format3)
{
    auto wrong_fmt = explode("%* %a%A %b%B %c%C %d%D %e%E %F %g%G %h%H %i%I %j%J %m%M %n%N %p%P %r%R %s%S %t%T %u%U %v%V %w%W %x%X %y%Y %z%Z %% %$%&%?%");
    auto right_fmt = explode("* %a%A %b%B %c%C %d%D %eE %F %g%G %h%H i%I %jJ %m%M %nN %pP %r%R s%S %t%T %u%U v%V %w%W %x%X %y%Y %z%Z %% $&?");
    
    std::string log_str;
    FakeRecordData rec;
    for (auto &f : wrong_fmt) {
        Formatter fmt("${time:"+ f +"}");
        log_str += fmt.format_record(&rec) + " ";
    } 
    std::string expected_str;
    for (auto &f : right_fmt) {
        expected_str += format_datatime(f.c_str(), rec.get_time()) + " ";
    }

    EXPECT_EQ(log_str, expected_str);
}


TEST(LogFormatterTest, get_format)
{
    const std::string format_str = "[${level_name}] ${file} ${line}: ${message}";
    Formatter fmt(format_str);

    EXPECT_EQ(fmt.get_format(), format_str);
}

TEST(LogFormatterTest, set_format)
{
    const std::string format_str = "[${level_name}] ${file} ${line}: ${message}";
    Formatter fmt("");
    FakeRecordData rec{LogLevel::INFO, "test", "test.cpp", 134};

    fmt.set_format(format_str);
    std::string line = fmt.format_record(&rec);

    EXPECT_EQ(line, "[INFO] test.cpp 134: test");
    EXPECT_EQ(fmt.get_format(), format_str);
}

TEST(LogFormatterTest, copy_format)
{
    FakeRecordData rec{LogLevel::WARNING, "test", "test.cpp", 1234};

    Formatter fmt1("[${level_name}] ${file} ${line}: ${message}");
    Formatter fmt2 = fmt1;
    Formatter fmt3("");
    fmt3 = fmt2;
    fmt1.set_format("");
    std::string line1 = fmt1.format_record(&rec);
    std::string line2 = fmt2.format_record(&rec);
    std::string line3 = fmt3.format_record(&rec);

    EXPECT_EQ(line1, "test");
    EXPECT_EQ(line2, "[WARNING] test.cpp 1234: test");
    EXPECT_EQ(line3, line2);
}

struct FormatterHolder
{
    std::unique_ptr<Formatter> fmt;
    
    FormatterHolder(Formatter&& formatter) 
        : fmt(std::make_unique<Formatter>(std::move(formatter)))
    { }
};

TEST(LogFormatterTest, move_format)
{
    FakeRecordData rec{LogLevel::WARNING, "test", "test.cpp", 1234};

    Formatter fmt1("[${level_name}] ${file} ${line}: ${message}");
    Formatter fmt2(std::move(fmt1));
    FormatterHolder hfmt(Formatter("[${level_name}]: ${message}"));
    std::string line1 = fmt1.format_record(&rec);
    std::string line2 = fmt2.format_record(&rec);
    std::string line3 = hfmt.fmt->format_record(&rec);

    EXPECT_EQ(line1, "test");
    EXPECT_EQ(line2, "[WARNING] test.cpp 1234: test");
    EXPECT_EQ(line3, "[WARNING]: test");
}
