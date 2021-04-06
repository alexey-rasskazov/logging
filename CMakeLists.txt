cmake_minimum_required(VERSION 3.8)

#
# UTF-8 Utilities
#

project(logger)

add_library(logger STATIC
  src/logger.cpp src/logger.h
  src/log_level.cpp src/log_level.h
  src/log_record.cpp src/log_record.h
  src/log_record_data.cpp src/log_record_data.h
  src/formatter.cpp src/formatter.h
  src/logger.cpp src/logger.h
  src/sink/base.cpp src/sink/base.h
  src/sink/cout.cpp src/sink/cout.h
  src/sink/file.cpp src/sink/file.h
  src/sink/helpers/file_writer.cpp src/sink/helpers/file_writer.h
  src/sink/helpers/filename_template.cpp src/sink/helpers/filename_template.h
)

set_target_properties(
  logger PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
)

# Google Test
include(scripts/gtest.cmake)

# Tests executable target
add_executable(unit_tests
  tests/format_tests.cpp
  tests/log_record_tests.cpp
  tests/sink_tests.cpp
  tests/file_tests.cpp
  tests/fake_record_data.cpp
)

add_executable(functional_tests
  tests/logging_tests.cpp
)

target_compile_features(unit_tests PRIVATE cxx_std_14)
target_compile_features(functional_tests PRIVATE cxx_std_14)

target_include_directories(unit_tests PUBLIC src)
target_include_directories(functional_tests PUBLIC src)

# Link gtest library
target_link_libraries(unit_tests gtest_main logger)
target_link_libraries(functional_tests gtest_main logger)

add_test(NAME unit_tests COMMAND unit_tests)
add_test(NAME functional_tests COMMAND functional_tests)

