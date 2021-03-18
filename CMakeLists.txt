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

#
# Google Test
#

# Download and unpack googletest at configure time
configure_file(scripts/google_test.cmake googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

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

