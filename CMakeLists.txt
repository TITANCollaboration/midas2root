cmake_minimum_required(VERSION 3.10)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(Midas2Root VERSION 1.0)

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net Core)
include(${ROOT_USE_FILE})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++1y -m64")

find_package(ZLIB)



add_executable(Midas2Root_mdpp16 midas2root_mdpp16.cxx)
add_executable(Midas2Root_grif16 midas2root_grif16.cxx)

target_link_libraries(Midas2Root_mdpp16)


#set_property(TARGET Midas2Root_mdpp16 PROPERTY CXX_STANDARD 11)

include_directories($ENV{ROOTANASYS}/include $ENV{ROOTSYS}/include $ENV{MIDASSYS}/include)

link_directories($ENV{ROOTANASYS}/libAnalyzer)
link_directories($ENV{ROOTANASYS}/lib)
link_directories($ENV{ROOTSYS}/lib)

set(LIBS $ENV{ROOTANASYS}/lib/librootana.a XMLParser $ENV{ROOTSYS}/lib/libGui.so $ENV{ROOTSYS}/lib/libRHTTP.so Z $ENV{MIDASSYS}/lib/libmidas.a ${ROOT_LIBRARIES} $ENV{ROOTANASYS}/libAnalyzer/libAnalyzer.a)

target_link_libraries(Midas2Root_mdpp16 ${LIBS})
target_link_libraries(Midas2Root_grif16 ${LIBS})
