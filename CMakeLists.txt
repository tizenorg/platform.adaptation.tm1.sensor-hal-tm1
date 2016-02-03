cmake_minimum_required(VERSION 2.6)
project(sensor-plugins CXX)
include(GNUInstallDirs)

# Common Options
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2 -omit-frame-pointer -std=gnu++0x")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdata-sections -ffunction-sections")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-section -Wl,--print-gc-section")
MESSAGE("FLAGS: ${CMAKE_CXX_FLAGS}")
MESSAGE("FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")

add_definitions(-DUSE_DLOG_LOG)
add_definitions(-DLIBDIR="${CMAKE_INSTALL_LIBDIR}")

# Internal Debugging Options
#add_definitions(-Wall -g -D_DEBUG)

# Sub-directory
add_subdirectory(src)
