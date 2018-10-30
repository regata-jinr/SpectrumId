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
set(CFLAGS " -g -ansi -pedantic -Wall -Wextra -Wno-sign-compare -Wsign-promo -Wabi -std=c++17 ")
#TODO: seems in cmake I should not specify such variables, but I can use find_package()
set(ROOT_CFLAGS " -pthread -m64 -I/usr/local/include") #root-config --cflags"
set(ROOT_LIBS " -L/usr/local/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -lm -ldl -rdynamic ") #"root-config --libs"
#TODO: seems in cmake I should not specify such variables, but I can use find_package()
set(GSL_CFLAGS " -I/usr/include ") # gsl-config --cflags
set(GSL_LIBS " -L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas -lm ") # gsl-config --libs
set(SOURCE_EXE spectrumId.cpp)

SET(CMAKE_CXX_FLAGS "${CFLAGS} ${ROOT_CFLAGS} ${GSL_CFLAGS}")

include_directories (inc)
include_directories (src)

add_executable(spectrumId ${SOURCE_EXE})
