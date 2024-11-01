cmake_minimum_required(VERSION 3.26)
project(counter)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

add_library(counter_lib STATIC counter_processor.cpp)

add_executable(counter main.cpp)

add_executable(counterTest tests.cpp)

target_link_libraries(counter PRIVATE counter_lib)

target_link_libraries(counterTest PRIVATE counter_lib gtest gtest_main)

add_test(NAME counterTest COMMAND counterTest)
