cmake_minimum_required(VERSION 3.9)
project(hashtable)

set(CMAKE_CXX_STANDARD 17)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O5")

add_subdirectory(lib/googletest-master)
#add_subdirectory(lib/benchmark)

include_directories(lib/googletest-master/googletest/include)
#include_directories(lib/benchmark/include)
include_directories(lib/json/include)

add_executable(hashtable main.cpp Share.h Day.h hashmap.h hashmap_impl.h hashmap_iter.h)
add_executable(runTests tests/test.cpp tests/tests.cpp tests/utils.h)
#add_executable(runBench bench/bm_hashtable.cpp)

target_link_libraries(hashtable gtest gtest_main)
target_link_libraries(runTests gtest gtest_main)
#target_link_libraries(runBench benchmark)
