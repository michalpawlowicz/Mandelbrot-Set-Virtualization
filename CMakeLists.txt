cmake_minimum_required(VERSION 3.6)
project(lab2)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -lm -Wpedantic")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(SOURCE_FILES_M master.c)
set(SOURCE_FILES_S slave.c)

add_executable(master ${SOURCE_FILES_M})
add_executable(slave ${SOURCE_FILES_S})

