cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

INCLUDE (FindPkgConfig)
pkg_check_modules (NDN_CXX libndn-cxx)

project(Sensor)

set(SOURCE_FILES main.cpp)
add_executable(Sensor ${SOURCE_FILES})

include_directories(Sensor ${NDN_CXX_INCLUDE_DIRS})
link_directories(Sensor ${NDN_CXX_LIBRARY_DIRS} )
target_link_libraries(Sensor ${NDN_CXX_LIBRARIES})