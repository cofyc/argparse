cmake_minimum_required(VERSION 2.9)
project(argparse C)

if(NOT DEFINED CMAKE_C_FLAGS)
	set(CMAKE_C_FLAGS "-O3")
endif()
if(NOT DEFINED CMAKE_C_FLAGS_DEBUG)
	if(UNIX)
	set(CMAKE_C_FLAGS_DEBUG "-g")
	else()
	set(CMAKE_C_FLAGS_DEBUG "-g -ggdb")
	endif()
endif()

set(sources argparse.c)

option(ARGPARSE_SHARED "Build shared library" ON)
option(ARGPARSE_STATIC "Build static library" ON)

if(ARGPARSE_SHARED)
	add_library(argparse_shared SHARED ${sources})
	set_target_properties(argparse_shared PROPERTIES OUTPUT_NAME argparse)
endif()
if(ARGPARSE_STATIC)
	add_library(argparse STATIC ${sources})
endif()
if(NOT (ARGPARSE_STATIC OR ARGPARSE_SHARED))
	add_library(argparse OBJECT ${sources})
endif()

option(ENABLE_TESTS "Enable tests" OFF)
if(ENABLE_TESTS OR CMAKE_TESTING_ENABLED)
	enable_testing()

	add_executable(test_argparse test_argparse.c ${sources})
	add_custom_command(
		TARGET test_argparse
		COMMENT "Running tests"
		POST_BUILD WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --output-on-failures)
endif()

if(UNIX)
	add_test(NAME argparse_test COMMAND ${PROJECT_SOURCE_DIR}/test.sh)
endif()
