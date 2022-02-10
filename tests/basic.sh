#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/tap-functions
plan_no_plan

is "$(./basic -f --path=/path/to/file a 2>&1)" 'force: 1
path: /path/to/file
argc: 1
argv[0]: a'

is "$(./basic -f -f --force --no-force 2>&1)" 'force: 2'

is "$(./basic -i 2>&1)" 'error: option `-i` requires a value'

is "$(./basic -i 2 2>&1)" 'int_num: 2'

is "$(./basic -i2 2>&1)" 'int_num: 2'

is "$(./basic -ia 2>&1)" 'error: option `-i` expects an integer value'

is "$(./basic -i 0xFFFFFFFFFFFFFFFFF 2>&1)" \
   'error: option `-i` numerical result out of range'

is "$(./basic -s 2.4 2>&1)" 'flt_num: 2.4'

is "$(./basic -s2.4 2>&1)" 'flt_num: 2.4'

is "$(./basic -sa 2>&1)" 'error: option `-s` expects a numerical value'

is "$(./basic -s 1e999 2>&1)" \
   'error: option `-s` numerical result out of range'

is "$(./basic -f -- do -f -h 2>&1)" 'force: 1
argc: 3
argv[0]: do
argv[1]: -f
argv[2]: -h'

is "$(./basic -tf 2>&1)" 'force: 1
test: 1'

is "$(./basic --read --write 2>&1)" 'perms: 3'

help_usage='Usage: basic [options] [[--] args]
   or: basic [options]

A brief description of what the program does and how it works.

    -h, --help            show this help message and exit

Basic options
    -f, --force           force to do
    -t, --test            test only
    -p, --path=<str>      path to read
    -i, --int=<int>       selected integer
    -s, --float=<flt>     selected float

Bits options
    --read                read perm
    --write               write perm
    --exec                exec perm

Additional description of the program after the description of the arguments.'

is "$(./basic -h)" "$help_usage"

is "$(./basic --help)" "$help_usage"

is "$(./basic --no-help 2>&1)" 'error: unknown option `--no-help`'$'\n'"$help_usage"
