cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

aux_source_directory(${PROJECT_SOURCE_DIR} RUN_TEST_SOURCES)

add_executable(run_test_array ${RUN_TEST_SOURCES})

#target_link_libraries(run_test_array )
