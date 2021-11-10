cmake_minimum_required(VERSION 3.0)

project(argparse VERSION 0.1.0 LANGUAGES C)

if(NOT CMAKE_C_FLAGS)
	set(CMAKE_C_FLAGS "-O3")
endif()
if(NOT CMAKE_C_FLAGS_DEBUG)
	set(CMAKE_C_FLAGS_DEBUG "-g -ggdb")
endif()

set(sources argparse.c)

option(ARGPARSE_SHARED "Build shared library" ON)
option(ARGPARSE_STATIC "Build static library" ON)

if(ARGPARSE_SHARED)
	add_library(argparse_shared SHARED ${sources})
	target_include_directories(argparse_shared PUBLIC .)
	set_target_properties(argparse_shared PROPERTIES OUTPUT_NAME argparse)
endif()
if(ARGPARSE_STATIC)
	add_library(argparse STATIC ${sources})
	target_include_directories(argparse PUBLIC .)
endif()
if(NOT (ARGPARSE_STATIC OR ARGPARSE_SHARED))
	add_library(argparse OBJECT ${sources})
endif()

option(ENABLE_TESTS "Enable tests" OFF)
if((ENABLE_TESTS OR CMAKE_TESTING_ENABLED) AND UNIX)
	enable_testing()

	add_executable(test_argparse test_argparse.c ${sources})
	add_test(NAME argparse_test COMMAND ${CMAKE_SOURCE_DIR}/test.sh)
	add_custom_command(
		TARGET test_argparse
		COMMENT "Running tests"
		POST_BUILD WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --output-on-failures)
endif()
