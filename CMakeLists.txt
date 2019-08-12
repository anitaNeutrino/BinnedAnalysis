# $Id: CMakeLists.txt 100923 2016-11-03 10:50:34Z gcosmo $

#



#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)
project(BinnedAnalysis)
# name project

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
unset(CMAKE_MODULE_PATH)


# Shouldn't need
find_package(ZLIB REQUIRED)
find_package(FFTW REQUIRED)

#find_package(GSL REQUIRED)
#use the ROOT things
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# minuit2 and whatever else
find_package(ROOT REQUIRED COMPONENTS RIO Tree MathMore)
include(${ROOT_USE_FILE})
#add_definitions(${ROOT_CXX_FLAGS})

if(DEFINED ENV{ANITA_UTIL_INSTALL_DIR})
    message("ANITA_UTIL_INSTALL_DIR is set to $ENV{ANITA_UTIL_INSTALL_DIR}")
    set(UTIL_LIB_DIR $ENV{ANITA_UTIL_INSTALL_DIR}/lib)
    set(UTIL_INC_DIR $ENV{ANITA_UTIL_INSTALL_DIR}/include)
    set(UTIL_BIN_DIR $ENV{ANITA_UTIL_INSTALL_DIR}/bin)
    set(UTIL_SHARE_DIR $ENV{ANITA_UTIL_INSTALL_DIR}/share)
    set(LD_UTIL $ENV{ANITA_UTIL_INSTALL_DIR}/lib)
    set(INC_UTIL $ENV{ANITA_UTIL_INSTALL_DIR}/include)
else()
    message("Will install to /usr/local  ANITA_UTIL_INSTALL_DIR not set")
    set(UTIL_LIB_DIR /usr/local/lib)
    set(UTIL_INC_DIR /usr/local/include)
    set(UTIL_BIN_DIR /usr/local/bin)
    set(UTIL_SHARE_DIR /usr/local/share)
    set(LD_UTIL /usr/local/lib)
    set(INC_UTIL /usr/local/include)
endif()




# include all the places we will find thing.  Get this line from Cosmin's
# include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/BinnedAnalysis ${ZLIB_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${BinnedAnalysis_INCLUDE_DIR} ${BinnedAnalysis_INCLUDE_DIR}/../)
include_directories(${PROJECT_SOURCE_DIR} ${ROOT_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/include ${FFTW_INCLUDES} ${ZLIB_INCLUDES} ${UTIL_INC_DIR})

#----------------------------------------------------------------------------
# cmake /path/to/source .(maybe) 
# once configuration done "make -B -J#cores"

file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/*.cxx )
file(GLOB HEADER_FILES ${BinnedAnalysis_INCLUDE_DIR}/*.h ${BinnedAnalysis_INCLUDE_DIR}/*.hh)# ${GSL_INCLUDE_DIRS}/gsl/*.h)

# figure this out
set(DICTNAME G__${libname})

ROOT_GENERATE_DICTIONARY(${DICTNAME} ${HEADER_FILES} LINKDEF LinkDef.h)


#---Create a shared library with geneated dictionary
add_library(${libname} SHARED ${SOURCE_FILES} ${DICTNAME}.cxx)
target_link_libraries(${libname} ${ZLIB_LIBRARIES} RootFftwWrapper AnitaEvent AnitaCorrelator AnitaAnalysis ${ROOT_LIBRARIES} ${FFTW_LIBRARIES})

#make the ROOT dictionaries and shared object library for reading the event tree


# the rest isn't needed now

# attempt to compile something during the cmake process
# add_executable(svdExample ${PROJECT_SOURCE_DIR}/example/svdExample.cc)
# add_executable(alignExample ${PROJECT_SOURCE_DIR}/example/alignExample.cc)
# target_link_libraries(svdExample ${ROOT_LIBRARIES} BinnedAnalysis)
# target_link_libraries(alignExample ${ROOT_LIBRARIES} BinnedAnalysis)
#----------------------------------------------------------------------------


# move files to target folder
# install (FILES ${BinnedAnalysis_INCLUDE_DIR}/BinnedAnalysisEvent.hh ${BinnedAnalysis_INCLUDE_DIR}/TAnalyze.hh ${BinnedAnalysis_INCLUDE_DIR}/TUtil.hh ${PROJECT_SOURCE_DIR}/include/cnpy.h ${BinnedAnalysis_INCLUDE_DIR}/TUtilGraph.hh ${BinnedAnalysis_INCLUDE_DIR}/TEventSummary.hh DESTINATION ${BinnedAnalysis_INSTALL_DIR}/include/BinnedAnalysis)
# #install (FILES ${PROJECT_SOURCE_DIR}/src/BinnedAnalysisEvent.cc DESTINATION ${BinnedAnalysis_INSTALL_DIR}/src/BinnedAnalysis)

# install (FILES ${PROJECT_SOURCE_DIR}/share/runLog.root DESTINATION ${BinnedAnalysis_INSTALL_DIR}/share/BinnedAnalysis)

# install (FILES ${PROJECT_BINARY_DIR}/libBinnedAnalysis_rdict.pcm DESTINATION ${LIB_INSTALL_DIR})
# install (TARGETS BinnedAnalysis LIBRARY DESTINATION ${LIB_INSTALL_DIR})
# install(TARGETS svdExample DESTINATION ${BinnedAnalysis_INSTALL_DIR}/share/BinnedAnalysis/example)
# install(TARGETS alignExample DESTINATION ${BinnedAnalysis_INSTALL_DIR}/share/BinnedAnalysis/example)

