#!/bin/bash

. tap-functions
plan_no_plan

is "$(./test_argparse -f --path=/path/to/file a 2>&1)" 'force: 1
path: /path/to/file
argc: 1
argv[0]: a'

is "$(./test_argparse -f -f --force 2>&1)" 'force: 3'

is "$(./test_argparse -n 2>&1)" 'error: option `n` requires a value'

is "$(./test_argparse -n 2 2>&1)" 'num: 2'

is "$(./test_argparse -n2 2>&1)" 'num: 2'

is "$(./test_argparse -na 2>&1)" 'error: option `n` expects a numerical value'

is "$(./test_argparse --help 2>&1)" 'Usage: test_argparse [options] [[--] args]

    -h, --help    show this help message and exit
    -f, --force   force to do
    -t, --test    test only
    -p, --path    path to read
    -n, --num     selected num
    --read        read perm
    --write       write perm
    --exec        exec perm'

is "$(./test_argparse -f -- do -f -h 2>&1)" 'force: 1
argc: 3
argv[0]: do
argv[1]: -f
argv[2]: -h'

is "$(./test_argparse -tf 2>&1)" 'force: 1
test: 1'

is "$(./test_argparse --read --write 2>&1)" 'perms: 3'
