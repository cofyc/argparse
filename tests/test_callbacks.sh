#!/bin/bash

. $(dirname "$0")/tap-functions

plan_tests 10

# Compile the test
gcc -o test_callbacks test_callbacks.c ../argparse.c || bailout "Failed to compile test_callbacks"

# Test 1: No callbacks triggered
output=$(./test_callbacks 2>&1)
is "$?" "0" "No callbacks - exit code 0"
echo "$output" | grep -q "callback_count: 0"
is "$?" "0" "No callbacks triggered"

# Test 2: Simple boolean callback
output=$(./test_callbacks --force 2>&1)
is "$?" "0" "Boolean callback - exit code 0"
echo "$output" | grep -q "callback_count: 1"
is "$?" "0" "One callback triggered"
echo "$output" | grep -q "callback_results: force,"
is "$?" "0" "Callback recorded force option"

# Test 3: Value callback with string
output=$(./test_callbacks --path=/tmp/test 2>&1)
is "$?" "0" "String callback - exit code 0"
echo "$output" | grep -q "callback_results: /tmp/test:path,"
is "$?" "0" "Callback recorded path value"

# Test 4: Multiple callbacks
output=$(./test_callbacks --force --path=/tmp --name=test 2>&1)
is "$?" "0" "Multiple callbacks - exit code 0"
echo "$output" | grep -q "callback_count: 3"
is "$?" "0" "Three callbacks triggered"
echo "$output" | grep -q "force,/tmp:path,test:name,"
is "$?" "0" "All callbacks recorded in order"

# Cleanup
rm -f test_callbacks
