#!/bin/bash

. $(dirname "$0")/tap-functions

plan_tests 6

# Compile the test
gcc -o test_stress test_stress.c ../argparse.c || bailout "Failed to compile test_stress"

# Test 1: Basic functionality with many options
output=$(./test_stress --flag-a --flag-z --str0=hello --int5=42 --float3=3.14 2>&1)
is "$?" "0" "Stress test with multiple options - exit code 0"
echo "$output" | grep -q "stress_test_completed: yes"
is "$?" "0" "Stress test completed successfully"

# Test 2: Help system works with many options
output=$(./test_stress --help 2>&1)
is "$?" "0" "Help works with many options"
echo "$output" | grep -q "Boolean flags"
is "$?" "0" "Group headers displayed in help"

# Test 3: Many short options combined (skip h since it conflicts with help)
output=$(./test_stress -abcdefgij 2>&1)
is "$?" "0" "Multiple short options work"
echo "$output" | grep -q "active_flags: 9"
is "$?" "0" "All short flags were parsed correctly"

# Cleanup
rm -f test_stress