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

target_compile_features(unit_tests PRIVATE cxx_std_14)
target_compile_features(functional_tests PRIVATE cxx_std_14)

target_include_directories(unit_tests PUBLIC ../include)
target_include_directories(functional_tests PUBLIC ../include)

# Link gtest library
target_link_libraries(unit_tests gtest_main logging)
target_link_libraries(functional_tests gtest_main logging)

add_test(NAME unit_tests COMMAND unit_tests)
add_test(NAME functional_tests COMMAND functional_tests)