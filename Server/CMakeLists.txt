cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

INCLUDE (FindPkgConfig)
pkg_check_modules (NDN_CXX libndn-cxx)

project(Server)

set(SOURCE_FILES main.cpp)
add_executable(Server ${SOURCE_FILES})

include_directories(Server ${NDN_CXX_INCLUDE_DIRS})
link_directories(Server ${NDN_CXX_LIBRARY_DIRS} )
target_link_libraries(Server ${NDN_CXX_LIBRARIES})