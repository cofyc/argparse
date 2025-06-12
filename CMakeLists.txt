cmake_minimum_required(VERSION 3.5)

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
	add_library(argparse_static STATIC ${sources})
	target_include_directories(argparse_static PUBLIC .)
	set_target_properties(argparse_static PROPERTIES OUTPUT_NAME argparse_static)
endif()

# Installation
include(GNUInstallDirs)

# Install header
install(FILES argparse.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Install libraries
if(ARGPARSE_SHARED)
	install(TARGETS argparse_shared
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(ARGPARSE_STATIC)
	install(TARGETS argparse_static
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

# Generate and install pkg-config file
configure_file(argparse.pc.in argparse.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/argparse.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

