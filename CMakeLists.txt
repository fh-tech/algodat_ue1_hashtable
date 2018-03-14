cmake_minimum_required(VERSION 3.9)
project(hashtable)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

add_executable(hashtable main.cpp Share.h Hashtable.h Day.h)
add_executable(runTests tests/test.cpp tests/tests.cpp)

target_link_libraries(hashtable gtest gtest_main)
target_link_libraries(runTests gtest gtest_main)