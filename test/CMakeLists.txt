# Google Test
include(scripts/gtest.cmake)

# Tests executable target
add_executable(unit_tests
    format_tests.cpp
    log_record_tests.cpp
    sink_tests.cpp
    file_tests.cpp
    fake_record_data.cpp
)

add_executable(functional_tests
    logging_tests.cpp
)

set_target_properties(
    unit_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
set_target_properties(
    functional_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(unit_tests PUBLIC ../include)
target_include_directories(functional_tests PUBLIC ../include)

# Link gtest library
target_link_libraries(unit_tests gtest_main logging)
target_link_libraries(functional_tests gtest_main logging)

add_test(NAME unit_tests COMMAND unit_tests)
add_test(NAME functional_tests COMMAND functional_tests)