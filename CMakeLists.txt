cmake_minimum_required(VERSION 3.1)
project(safe_mutex)
enable_testing()

add_subdirectory(googletest)

config_compiler_and_linker()
add_test(NAME test_safe_mutex COMMAND test_safe_mutex)
add_executable(test_safe_mutex tests/test_safe_mutex.cpp)
target_include_directories(test_safe_mutex PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(test_safe_mutex gtest)
