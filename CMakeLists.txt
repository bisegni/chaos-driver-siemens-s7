cmake_minimum_required(VERSION 2.6)
option(BUILD_FORCE_32 "Set to ON to enable 32 bit compilation" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

IF( ($ENV{CHAOS32}) OR (BUILD_FORCE_32) )
    MESSAGE(STATUS "Enabling 32 bit Compilation")
    set (CMAKE_C_FLAGS "-m32")
    set (CMAKE_CXX_FLAGS "-m32")
    set (CMAKE_LINK_FLAGS "-m32")
ENDIF()
ADD_DEFINITIONS(-g -O2 -Wall)
add_definitions(-DBOOST_ALL_DYN_LINK)

SET(siemens_s7_driver_src src/S7TcpDriver.cpp src/s7lib/nodave.c src/s7lib/openSocket.c src/s7lib/setport.c)
SET(siemens_s7_cu_src src/main.cpp src/S7ControlUnit.cpp ${siemens_s7_driver_src})

INCLUDE_DIRECTORIES(. ${PROJECT_SOURCE_DIR}/../../usr/local/include)
LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/../../chaosframework/usr/local/lib)

ADD_EXECUTABLE(SiemensS7ControlUnit ${siemens_s7_cu_src})
ADD_LIBRARY(SiemensS7DeviceDriver SHARED ${siemens_s7_driver_src})
set_target_properties(SiemensS7DeviceDriver PROPERTIES PREFIX "" SUFFIX ".extension")

TARGET_LINK_LIBRARIES(SiemensS7ControlUnit chaos_cutoolkit chaos_common boost_program_options boost_system boost_thread boost_chrono boost_regex boost_log boost_log_setup boost_atomic memcached msgpack msgpack-rpc mpio)
TARGET_LINK_LIBRARIES(SiemensS7DeviceDriver chaos_cutoolkit chaos_common boost_program_options boost_system boost_thread boost_chrono boost_regex boost_log boost_log_setup boost_atomic memcached msgpack msgpack-rpc mpio)

INSTALL_TARGETS(${PROJECT_SOURCE_DIR}/bin SiemensS7ControlUnit)
INSTALL_TARGETS(${PROJECT_SOURCE_DIR}/lib SiemensS7DeviceDriver)