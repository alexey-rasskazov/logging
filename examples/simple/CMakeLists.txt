set(TARGET_NAME example_simple)

project(${TARGET_NAME})

# Example executable target
add_executable(${TARGET_NAME} main.cpp)

set_target_properties(
    ${TARGET_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${TARGET_NAME} PUBLIC ../../include)

target_link_libraries(${TARGET_NAME} logging)
