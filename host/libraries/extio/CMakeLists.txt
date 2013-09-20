# Copyright 2012 Nuand, LLC.
#
# extio.dll support
#
# This file is part of the bladeRF project
#
cmake_minimum_required(VERSION 2.8)
project(extio CXX C)

# This is a windows-only library
if(WIN32)
    message(STATUS "Configured to build extio.dll")
else(WIN32)
    return()
endif()

################################################################################
# Version information
################################################################################

set(VERSION_INFO_MAJOR  0)
set(VERSION_INFO_MINOR  1)
set(VERSION_INFO_PATCH  0)
set(VERSION_INFO_EXTRA "git")
include(Version)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/version.h
    @ONLY
)

################################################################################
# Include paths
################################################################################

set(EXTIO_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${libbladeRF_SOURCE_DIR}/include
        ${BLADERF_HOST_COMMON_INCLUDE_DIRS}
)

include_directories(${EXTIO_INCLUDE_DIRS})

################################################################################
# Source files
################################################################################
set(EXTIO_SOURCE
        src/extio_bladerf.cpp
)

add_library(extio SHARED ${EXTIO_SOURCE})
target_link_libraries(extio libbladerf_shared)
