cmake_minimum_required(VERSION 3.12)

#
# Logging
#

project(logging)

set(LOGGING_INCLUDE_DIR include)

set(PUBLIC_HEADERS
    logger.h
    log_level.h
    log_record.h
    formatter.h
    helper/datetime.h
    # sinks
    sink/base.h
    sink/cout.h
    sink/file.h
)
list(TRANSFORM PUBLIC_HEADERS PREPEND "${LOGGING_INCLUDE_DIR}/logging/")

set(LOGGING_SOURCES
    logger.cpp
    log_level.cpp
    log_record.cpp
    formatter.cpp
    helper/log_record_data.cpp
    helper/datetime.cpp
    sink/base.cpp
    sink/cout.cpp
    sink/file.cpp
    sink/helpers/file_writer.h
    sink/helpers/file_writer.cpp
    sink/helpers/filename_template.h
    sink/helpers/filename_template.cpp
)
list(TRANSFORM LOGGING_SOURCES PREPEND "src/")

add_library(logging STATIC ${PUBLIC_HEADERS} ${LOGGING_SOURCES})

set_target_properties(
    logging PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_compile_definitions(logging PUBLIC _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

target_include_directories(logging PUBLIC ${LOGGING_INCLUDE_DIR})

if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )

    add_subdirectory(test)

endif()
