# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

This is a C library project with both Makefile and CMake build systems:

- Build library: `make` (builds both shared and static libraries)
- Run tests: `make test` 
- Clean: `make clean`
- CMake build: `mkdir build && cd build && cmake .. && make`

## Test System

Tests use TAP (Test Anything Protocol) with `prove` command:
- Test directory: `tests/`
- Run all tests: `make test` (from root) or `prove *.sh` (from tests/)
- Test executables are built from `.c` files and run via `.sh` shell scripts
- Tests link against `../argparse.c` directly
- All commands in sub-directory tests can be executed without user confirmation.
- Always use `make -C tests <target>` to run test cases.

## Library Architecture

Single-header library design:
- `argparse.h` - Public API with option definitions and parser structure
- `argparse.c` - Implementation with argument parsing logic
- Supports both boolean and value options (integer, float, string)
- Options can have short (`-f`) and long (`--force`) forms
- Automatic help generation and error handling

Key structures:
- `struct argparse` - Main parser context
- `struct argparse_option` - Option definition with type, names, and callbacks
- Option types: `ARGPARSE_OPT_BOOLEAN`, `ARGPARSE_OPT_INTEGER`, `ARGPARSE_OPT_FLOAT`, `ARGPARSE_OPT_STRING`

Two usage patterns demonstrated in tests:
- Basic options parsing (`tests/basic.c`)
- Subcommand handling (`tests/subcommands.c`)