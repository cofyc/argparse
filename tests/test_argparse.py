import subprocess
from pathlib import Path
from typing import List
import pytest

TEST_DIR = Path(__file__).parent
SUCCESS = 0

def run(params: List[str]):
    proc = subprocess.Popen(f"{TEST_DIR / 'subcommands'} {' '.join(params)}", stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    stdout, stderr = proc.communicate()
    return stdout.decode('utf-8'),  stderr.decode('utf-8'), proc.returncode

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

def test_no_params_show_usages():
    stdout, _, exit_code = run([])
    assert SUBCOMMANDS_USAGE in stdout
    assert exit_code == SUCCESS

def test_no_params_show_usages_subcommand():
    stdout, _, exit_code = run(["inspect"])
    assert INSPECT_USAGE in stdout
    assert exit_code == SUCCESS

def test_unknown_param():
    UNKNOWN_COMMAND = "sagasgsagas"
    stdout, stderr, exit_code = run([UNKNOWN_COMMAND])
    assert SUBCOMMANDS_USAGE in stdout
    assert f"error: unknown option `{UNKNOWN_COMMAND}`" in stderr
    assert exit_code != SUCCESS

@pytest.mark.skip
def test_opt_group():
    pass

@pytest.mark.skip
def test_opt_boolean():
    pass

@pytest.mark.skip
def test_opt_bit():
    pass

@pytest.mark.skip
def test_opt_integer():
    pass

@pytest.mark.skip
def test_opt_float():
    pass

@pytest.mark.skip
def test_opt_string():
    pass

@pytest.mark.skip
def test_free_args():
    # test params not to be parsed
    # given by separating using --
    pass

@pytest.mark.skip
def test_option_callback():
    pass