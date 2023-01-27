cmake_minimum_required(VERSION 3.14)

set (CMAKE_CXX_STANDARD 11)

# For Clang to do parsing
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the project name
project(ssTestProject)

# ==========================================================
# Variables 
# ==========================================================
set(CMAKE_FIND_DEBUG_MODE 1)
set(ssTEST_BUILD_CHECK "ON" CACHE BOOL "Use and create static library instead")

add_library(ssTest INTERFACE)
target_include_directories(ssTest INTERFACE "${CMAKE_CURRENT_LIST_DIR}/Include")
target_sources(ssTest INTERFACE
                        "${CMAKE_CURRENT_LIST_DIR}/Include/ssTest.hpp"
                        )

if(CMAKE_FIND_DEBUG_MODE)
    add_executable(ssTestCheck "${CMAKE_CURRENT_LIST_DIR}/Src/ssTestCheck.cpp")
    target_link_libraries(ssTestCheck PUBLIC ssTest)
endif()
