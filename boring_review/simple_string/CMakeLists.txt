cmake_minimum_required(VERSION 2.6)

#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

#include_directories(inc)

aux_source_directory(${PROJECT_SOURCE_DIR} RUN_TEST_SOURCES)

add_executable(run_test_string ${RUN_TEST_SOURCES})

target_link_libraries(run_test_string)
