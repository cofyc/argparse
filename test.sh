#!/bin/bash

test -f test_argparse || make

. tap-functions
plan_no_plan

is "$(./test_argparse -f --path=/path/to/file a)" 'force: 1
path: /path/to/file
num: 0
argc: 1
argv[0]: a'

is "$(./test_argparse -n 2>&1)" 'error: option `n` requires a value
force: 0
path: (null)
num: 0
argc: 0'

is "$(./test_argparse -n 2 2>&1)" 'force: 0
path: (null)
num: 2
argc: 0'

is "$(./test_argparse -n2 2>&1)" 'force: 0
path: (null)
num: 2
argc: 0'
