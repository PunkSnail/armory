
cmake_minimum_required(VERSION 2.6)

#set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -D_DEBUG -Wall")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wconversion -g")

include_directories(inc)

aux_source_directory(src RUN_TEST_SOURCES)

set (RUN_TEST_SOURCES ${RUN_TEST_SOURCES})

add_executable(formatted_text ${RUN_TEST_SOURCES})

target_link_libraries(formatted_text)

