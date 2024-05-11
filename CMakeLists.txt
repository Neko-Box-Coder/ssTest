cmake_minimum_required(VERSION 3.14)

if(TARGET ssTest)
    return()
endif()

set (CMAKE_CXX_STANDARD 11)

# For Clang to do parsing
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the project name
project(ssTestProject)

# ==========================================================
# Variables 
# ==========================================================
set(ssTEST_BUILD_CHECK "OFF" CACHE BOOL "Use and create static library instead")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(STANDARD_COMPILE_FLAGS "/utf-8;/W1")
else()
    set(STANDARD_COMPILE_FLAGS  "-Wall"
                                "-Wno-return-local-addr"
                                "-Wno-sign-compare"
                                "-Wno-unused-variable"
                                "-Wno-unused-parameter"
                                "-Wno-switch"
                                "-Wextra"
                                "-pedantic"
                                "-Werror")
endif()

add_library(ssTest INTERFACE)
target_include_directories(ssTest INTERFACE "${CMAKE_CURRENT_LIST_DIR}/Include")
target_sources(ssTest INTERFACE
                        "${CMAKE_CURRENT_LIST_DIR}/Include/ssTest.hpp"
                        )

if(ssTEST_BUILD_CHECK)
    add_executable(ssTestCheck "${CMAKE_CURRENT_LIST_DIR}/Src/ssTestCheck.cpp")
    add_executable(ssTestExample "${CMAKE_CURRENT_LIST_DIR}/Src/ssTestExample.cpp")
    target_link_libraries(ssTestCheck PUBLIC ssTest)
    target_link_libraries(ssTestExample PUBLIC ssTest)
    target_compile_options(ssTestCheck PUBLIC "${STANDARD_COMPILE_FLAGS}")
    target_compile_options(ssTestExample PUBLIC "${STANDARD_COMPILE_FLAGS}")
endif()
