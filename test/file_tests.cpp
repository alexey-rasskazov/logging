#include "gtest/gtest.h"
#include <logging/logger.h>
#include <logging/sink/file.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "fake_record_data.h"

using namespace logging;

/**
 * @brief File test class
 * 
 */
class FileTest : public ::testing::Test
{
protected:

    void SetUp() override 
    {
        SetUp(file_template);
    }

    void SetUp(const std::string& file_template, int limit = 0)
    {
        this->file_template = file_template;
        file_sink = std::make_unique<FileSink>(file_template, limit);
    }

    void TearDown() override
    {
        std::string filename = file_sink->get_filename();
        file_sink.reset(nullptr);
        for (auto& name : filenames) {
            std::remove(name.c_str());
        }

        std::remove(filename.c_str());
    }

    void add_file(const std::string& filename)
    {
        filenames.push_back(filename);
    }
    
    std::string read_file(const char* file = nullptr)
    {
        std::fstream file_stream;
        file_stream.open(file ? file : file_sink->get_filename().c_str(), std::ios::in);
        if (file_stream.is_open()) {
            return std::string((std::istreambuf_iterator<char>(file_stream)), std::istreambuf_iterator<char>());
        } else {
            return "";
        }
    }

    std::unique_ptr<FileSink> file_sink;
    std::string file_template;
    std::vector<std::string> filenames;

    /**
     * @brief Construct a new FileTest object
     * 
     */
    FileTest() : 
        file_template("test_logs/test.log")
    { }

private:
    
    std::stringstream buffer;
    std::streambuf *external_buf;
};

TEST_F(FileTest, empty)
{
    FakeRecordData record;
    file_sink->write(&record, nullptr);

    EXPECT_EQ(read_file(), "\n");
}

TEST_F(FileTest, single_line)
{
    FakeRecordData record(LogLevel::INFO, "test test");
    file_sink->write(&record, nullptr);

    EXPECT_EQ(read_file(), "test test\n");
}

TEST_F(FileTest, multiple_lines)
{
    std::string expected_data;
    for (int i = 0; i < 10; ++i)
    {
        std::string line = "line_" + std::to_string(i + 1);
        FakeRecordData record(LogLevel::INFO, line.c_str());
        file_sink->write(&record, nullptr);
        expected_data.append(line);
        expected_data.append("\n");
    }

    EXPECT_EQ(read_file(), expected_data);
}

TEST_F(FileTest, rotate_files_by_day)
{
    SetUp("test_logs/rotation_test_day_%Y-%m-%d.log");
    FakeRecordData record1(LogLevel::INFO, "line_1");
    FakeRecordData record2(LogLevel::INFO, "line_2", "test.cpp", 0, record1.milliseconds + (24 + 2) * 3600 * 1000);

    file_sink->write(&record1, nullptr);
    add_file(file_sink->get_filename());
    file_sink->write(&record2, nullptr);

    EXPECT_EQ(read_file(filenames[0].c_str()), "line_1\n");
    EXPECT_EQ(read_file(), "line_2\n");
}

TEST_F(FileTest, rotate_files_by_hour)
{
    SetUp("test_logs/log_by_hour/rotation_test_hour_%Y-%m-%d_%H.log");
    std::string expected_data;
    FakeRecordData record;
    std::vector<std::string> expectations;

    for (int i = 0; i < 100; ++i) {
        record.data = "line" + std::to_string(i + 1);
        record.milliseconds += 5 * 60 * 1000;
        file_sink->write(&record, nullptr);
        if (filenames.empty() || filenames.back() != file_sink->get_filename()) {
            add_file(file_sink->get_filename());
            if (!expected_data.empty()) {
                expectations.push_back(expected_data);
                expected_data.clear();
            }
        }
        expected_data.append(record.data + "\n");
    }
    expectations.push_back(expected_data);

    EXPECT_GE(filenames.size(), 6);
    EXPECT_LE(filenames.size(), 10);

    for (size_t i = 0; i < filenames.size(); ++i) {
        EXPECT_EQ(read_file(filenames[i].c_str()), expectations[i]);
    }
}

TEST_F(FileTest, rotate_files_max_5)
{
    std::string path = "test_logs/log_max_5/";
    std::filesystem::remove_all(path);
    SetUp(path + "rotation_test_hour_%Y-%m-%d_%H.log", 5);
    FakeRecordData record;

    for (int i = 0; i < 100; ++i) {
        record.data = "line " + std::to_string(i + 1);
        record.milliseconds += 5 * 60 * 1000;
        file_sink->write(&record, nullptr);
        if (filenames.empty() || filenames.back() != file_sink->get_filename()) {
            add_file(file_sink->get_filename());
        }
    }

    EXPECT_GE(filenames.size(), 6);
    EXPECT_LE(filenames.size(), 10);

    std::filesystem::path dir{filenames[0]};
    dir.remove_filename();
    size_t count = 0;
    for (auto& p: std::filesystem::directory_iterator(dir)) {
        if (p.is_regular_file()) {
            ++count;
        }
    }
    EXPECT_EQ(count, 5);
}

TEST_F(FileTest, template_wrong_syntax)
{
    try {
        SetUp("test%_logs/%Y/rotation_test_hour_%Y-%m-%d_%H.log");
        FAIL();
    } catch (FileTemplateException &e) {
        SUCCEED();
    } catch(...) {
        FAIL();
    }
}

TEST_F(FileTest, template_empty)
{
    try {
        SetUp("");
        FAIL();
    } catch (FileTemplateException &e) {
        SUCCEED();
    } catch(...) {
        FAIL();
    }
}

TEST_F(FileTest, template_end_slash)
{
    try {
        SetUp("test_logs/%Y/rotation_test_hour_%Y-%m-%d_%H/");
        FAIL();
    } catch (FileTemplateException &e) {
        SUCCEED();
    } catch(...) {
        FAIL();
    }
}