#!/bin/bash

. $(dirname ${BASH_SOURCE[0]})/tap-functions
plan_no_plan

is "$(./subcommands foo)" 'executing subcommand foo
argc: 1
argv[0]: foo
after argparse_parse:
argc: 0'

is "$(./subcommands foo -t -p /path/to/file arg1)" 'executing subcommand foo
argc: 5
argv[0]: foo
argv[1]: -t
argv[2]: -p
argv[3]: /path/to/file
argv[4]: arg1
after argparse_parse:
argc: 1
argv[0]: arg1'
