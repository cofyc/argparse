#!/bin/bash

. $(dirname "$0")/tap-functions

plan_tests 3

# Compile the test
gcc -o test_null_help test_null_help.c ../argparse.c || bailout "Failed to compile test_null_help"

# Test 1: Check that --help works without crashing when options have NULL help
output=$(./test_null_help --help 2>&1)
ret=$?
is "$ret" "0" "Help exits with 0"

# Test 2: Verify help output contains options (even with no description)
echo "$output" | grep -q -- "-f, --force"
is "$?" "0" "Help output contains -f, --force option"

# Test 3: Verify option with help text shows correctly
echo "$output" | grep -q "enable verbose output"
is "$?" "0" "Help output contains verbose option description"

# Cleanup
rm -f test_null_help