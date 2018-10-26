# **************************************************************************
# *                                                                        *
# *                                                                        *
# * Copyright(c) 2018, REGATA Experiment at JINR, All rights reserved.     *
# *                                                                        *
# *                                                                        *
# **************************************************************************
 
#--------------------------------------------------------------------------#
# Set Basic CMake Configuration                                            #
#--------------------------------------------------------------------------#

cmake_minimum_required(VERSION 3.10.2 FATAL_ERROR)

project(spectrumId  CXX C)	

set(ROOT_VERSION_MIN_6 "6.15.01")
set(CMAKE_CXX_STANDARD 17)
#TODO: clearify is it need to have so many flags?
set(CFLAGS "-g -ansi -pedantic -Wall -Wextra -Wno-sign-compare -Wsign-promo -Wabi -std=c++17")
#TODO: seems in cmake I should not specify such variables, but I can use find_package()
set(ROOT_CFLAGS "root-config --cflags")
set(ROOT_LIBS "root-config --libs")
#TODO: seems in cmake I should not specify such variables, but I can use find_package()
set(GSL_CFLAGS "gsl-config --cflags")
set(GSL_LIBS "gsl-config --libs")

set(SOURCE_EXE spectrumId.cpp)
add_executable(spectrumId ${SOURCE_EXE})
