cmake_minimum_required(VERSION 2.6)                                                                                  

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_DEBUG -Wconversion -g -Wall")

#include_directories(inc)

aux_source_directory(${PROJECT_SOURCE_DIR} RUN_TEST_SOURCES)

add_executable(run_test ${RUN_TEST_SOURCES})

target_link_libraries(run_test)

