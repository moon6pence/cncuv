cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)
if (POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif (POLICY CMP0043)

project(cncuv)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(cncuv.test test.cpp cnc.h)

