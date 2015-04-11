cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0020 NEW)
if (POLICY CMP0043)
    cmake_policy(SET CMP0043 NEW)
endif (POLICY CMP0043)

project(cncuv)

include_directories("${PROJECT_SOURCE_DIR}")

# libuv
include_directories("${PROJECT_SOURCE_DIR}/libuv/include")
link_directories("${PROJECT_SOURCE_DIR}/libuv/lib")

add_executable(cncuv.test 
    test.cpp 
    cnc/TagCollection.h
    cnc/ItemCollection.h
    cnc/StepCollection.h
    cnc/StepLauncher.h
    cnc/cnc.h)

target_link_libraries(cncuv.test libuv Ws2_32 psapi Iphlpapi)