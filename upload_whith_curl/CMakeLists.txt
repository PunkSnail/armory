cmake_minimum_required(VERSION 2.6)

project(upload_whith_curl)

set (CMAKE_C_COMPILER ${CMAKE_CXX_COMPILER}) 
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG -g -Wall \
                        -fPIC -fvisibility=hidden")
include_directories(inc ../common)

set (UPLOAD_SOURCES src/upload_whith_curl.cpp)

add_library(upload_whith_curl SHARED ${UPLOAD_SOURCES})

target_link_libraries(upload_whith_curl curl pthread)

