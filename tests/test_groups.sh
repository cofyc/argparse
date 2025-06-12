#!/bin/bash

. $(dirname "$0")/tap-functions

plan_tests 8

# Compile the test
gcc -o test_groups test_groups.c ../argparse.c || bailout "Failed to compile test_groups"

# Test 1: Check that help displays all groups
output=$(./test_groups 2>&1)
is "$?" "0" "Help display exits with 0"

# Test 2-5: Verify each group header appears
echo "$output" | grep -q "General options"
is "$?" "0" "General options group displayed"

echo "$output" | grep -q "Input/Output options"
is "$?" "0" "Input/Output options group displayed"

echo "$output" | grep -q "Processing options"
is "$?" "0" "Processing options group displayed"

# Test 6: Verify options appear under correct groups
echo "$output" | grep -A3 "General options" | grep -q -- "--verbose"
is "$?" "0" "verbose option under General options"

echo "$output" | grep -A3 "Input/Output options" | grep -q -- "--input"
is "$?" "0" "input option under Input/Output options"

# Test 7: Verify help formatting with groups
echo "$output" | grep -q -- "-h, --help"
is "$?" "0" "Help option displayed correctly"

# Test 8: Test actual parsing still works
output=$(./test_groups --verbose --input=test.txt 2>&1)
is "$?" "0" "Options parse successfully with groups"

# Cleanup
rm -f test_groups