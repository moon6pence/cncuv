cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)
if (POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif (POLICY CMP0043)

project(cncuv)

include_directories("${PROJECT_SOURCE_DIR}")

# Compiler option for gcc
if (${UNIX})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )
endif (${UNIX})

# libuv
include_directories("${PROJECT_SOURCE_DIR}/libuv/include")
link_directories("${PROJECT_SOURCE_DIR}/libuv/lib")

# cncuv
add_library(cncuv
    cnc/ItemCollection.h
    cnc/StepCollection.h
    cnc/StepLauncher.h
    cnc/cnc.h
    cnc/cnc.cpp)

if (${UNIX})
    target_link_libraries(cncuv uv)
elseif (${WIN32})
    target_link_libraries(cncuv libuv Ws2_32 psapi Iphlpapi)
endif (${UNIX})

# cncuv.test
add_executable(cncuv.test test.cpp)
target_link_libraries(cncuv.test cncuv)

# fib
add_executable(fib example/fib.cpp)
target_link_libraries(fib cncuv)

# empty_task
add_executable(empty_task example/empty_task.cpp)
target_link_libraries(empty_task cncuv)
