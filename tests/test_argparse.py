from conftest import SUBCOMMANDS_USAGE, run, SUCCESS, INSPECT_USAGE, SUBCOMMANDS_EXECUTABLE
import pytest


def test_no_params_show_usages():
    stdout, _, exit_code = run(SUBCOMMANDS_EXECUTABLE,[])
    assert SUBCOMMANDS_USAGE in stdout
    assert exit_code == SUCCESS

def test_no_params_show_usages_subcommand():
    stdout, _, exit_code = run(SUBCOMMANDS_EXECUTABLE, ["inspect"])
    assert INSPECT_USAGE in stdout
    assert exit_code == SUCCESS

def test_unknown_param():
    UNKNOWN_COMMAND = "sagasgsagas"
    stdout, stderr, exit_code = run(SUBCOMMANDS_EXECUTABLE, [UNKNOWN_COMMAND])
    assert SUBCOMMANDS_USAGE in stdout
    assert f"error: unknown option `{UNKNOWN_COMMAND}`" in stderr
    assert exit_code != SUCCESS



@pytest.mark.skip
def test_free_args():
    # test params not to be parsed
    # given by separating using --
    pass