from pathlib import Path
from typing import List
import subprocess

TEST_DIR = Path(__file__).parent
SUCCESS = 0

def run(executable: str, params: List[str]):
    proc = subprocess.Popen(f"{TEST_DIR / executable} {' '.join(params)}", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    stdout, stderr = proc.communicate()
    return stdout.decode('utf-8'),  stderr.decode('utf-8'), proc.returncode

BASIC_USAGE = \
"""Usage: basic [options] [[--] args]
   or: basic [options]

A brief description of what the program does and how it works.

Options:
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
    --exec                exec perm"""

SUBCOMMANDS_EXECUTABLE = "subcommands"
SUBCOMMANDS_USAGE = \
"""Usage: subcommands [options] [cmd] [args]

Options:
    -h, --help    show this help message and exit

Commands:
    inspect        inspects the system
    print_argv     prints argv"""

INSPECT_USAGE = \
"""Usage: inspect [options] [cmd] [args]

Options:
    -h, --help        show this help message and exit
    -f, --force       force to do
    -t, --test        test only
    -p, --path=<str>  path to read
"""