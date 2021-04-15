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

if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )

    add_subdirectory(tests)

endif()
