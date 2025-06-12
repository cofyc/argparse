#!/bin/bash

. $(dirname "$0")/tap-functions

plan_tests 10

# Compile the test
gcc -o test_ignore_unknown test_ignore_unknown.c ../argparse.c || bailout "Failed to compile test_ignore_unknown"

# Test 1: Normal operation with known args only
output=$(./test_ignore_unknown -v --file=test.txt 2>&1)
is "$?" "0" "Known args only - exit code 0"
echo "$output" | grep -q "verbose: 1"
is "$?" "0" "Verbose flag parsed correctly"
echo "$output" | grep -q "file: test.txt"
is "$?" "0" "File option parsed correctly"

# Test 2: Unknown long option is ignored
output=$(./test_ignore_unknown --unknown-option value -v 2>&1)
is "$?" "0" "Unknown long option ignored - exit code 0"
echo "$output" | grep -q "error: unknown option"
is "$?" "0" "Error message displayed for unknown option"
echo "$output" | grep -q "verbose: 1"
is "$?" "0" "Known option still parsed after unknown"

# Test 3: Unknown short option is ignored
output=$(./test_ignore_unknown -x -v --file=foo 2>&1)
is "$?" "0" "Unknown short option ignored - exit code 0"
echo "$output" | grep -q "error: unknown option.*-x"
is "$?" "0" "Error message for unknown short option"

# Test 4: Mix of known and unknown options with non-option args
output=$(./test_ignore_unknown -v --unknown arg1 --file=bar arg2 2>&1)
is "$?" "0" "Mixed known/unknown options - exit code 0"
echo "$output" | grep -q "remaining_args:.*arg1.*arg2"
is "$?" "0" "Non-option args passed through correctly"

# Cleanup
rm -f test_ignore_unknown