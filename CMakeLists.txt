# project name
project(ldb)

cmake_minimum_required(VERSION 2.6.0)

enable_testing()

# -- check for LevelDB backend
find_library(LDB_LIBRARY NAMES leveldb)
if(LDB_LIBRARY STREQUAL "LDB-NOTFOUND")
    message(FATAL_ERROR "Failed to find LevelDB libaries. Please install.")
endif()

find_package(GTest REQUIRED)

add_definitions(-std=c++0x)

# g++ compiller flags for debugging
if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -Wall")
endif()

# tell cmake to process CMakeLists.txt in these subdirs
add_subdirectory(src)
add_subdirectory(tests)