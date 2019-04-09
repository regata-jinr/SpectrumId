# **************************************************************************
# *                                                                        *
# *                                                                        *
# * Copyright(c) 2018-19, REGATA Experiment at JINR, All rights reserved.  *
# *                                                                        *
# *                                                                        *
# **************************************************************************

#--------------------------------------------------------------------------#
# Set Basic CMake Configuration                                            #
#--------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

project(spectrumID CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)

include(CheckCXXCompilerFlag)

OPTION (BUILD_SHARED_LIBS "Build shared libraries." ON)
message(_CMAKE_SOURCE_DIR_)
message(${CMAKE_SOURCE_DIR})
include_directories( ${CMAKE_SOURCE_DIR}/core/spectrum/inc )
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#TODO: e.g. -On? for optimisations
set(CFLAGS " -pthread -g -ansi -pedantic -Wall -Wextra -Wno-sign-compare -Wsign-promo -Wabi -std=c++17 -Wno-pragma-once-outside-header")
set(CMAKE_CXX_FLAGS ${CFLAGS})
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARIES})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#find_package(GSL REQUIRED)
#include_directories(${GSL_INCLUDE_DIR})
#link_directories(${GSL_LIBRARY_DIR})
enable_testing()

add_subdirectory(core)
add_subdirectory(main)
add_subdirectory(test)
