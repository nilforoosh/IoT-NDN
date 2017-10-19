cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

INCLUDE (FindPkgConfig)
pkg_check_modules (NDN_CXX libndn-cxx)

project(Client)

set(SOURCE_FILES main.cpp)
add_executable(Client ${SOURCE_FILES})

include_directories(Client ${NDN_CXX_INCLUDE_DIRS})
link_directories(Client ${NDN_CXX_LIBRARY_DIRS} )
target_link_libraries(Client ${NDN_CXX_LIBRARIES})