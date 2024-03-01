import pytest
import json
from conftest import run, BASIC_USAGE, SUCCESS

@pytest.mark.parametrize("help_param", ["-h", "--help"])
def test_help(help_param):
    stdout, _, exit_code = run("basic", [help_param])
    assert BASIC_USAGE in stdout
    assert exit_code == SUCCESS

def test_opt_group():
    stdout, _, exit_code = run("basic", ["-h"])
    assert "Basic options\n" in stdout
    assert "Bits options\n" in stdout
    assert exit_code == SUCCESS

@pytest.mark.parametrize("opts", [("force", 1), ("force", 0), ("test", 0), ("test", 1)])
@pytest.mark.parametrize("shortnames", [True, False])
def test_opt_boolean(opts, shortnames):
    expected_key, expected_val = opts
    if expected_val:
        params = f"--{expected_key}" if shortnames else f"-{expected_key[0]}"
    else:
        params = "--"
    stdout, _, exit_code = run("basic", [params])
    stdout = json.loads((stdout))
    assert stdout[expected_key] == expected_val
    assert exit_code == SUCCESS

@pytest.mark.parametrize("r", ["--read", ""])
@pytest.mark.parametrize("w", ["--write", ""])
@pytest.mark.parametrize("x", ["--exec", ""])
def test_opt_bit(r, w, x):
    if not r and not w and not x:
        return
    PERM_READ  = (1 << 0) if r else 0
    PERM_WRITE = (1 << 1) if w else 0
    PERM_EXEC  = (1 << 2) if x else 0
    stdout, _, exit_code = run("basic", [r, w, x])
    perms = int(json.loads(stdout)["perms"])
    assert (PERM_WRITE | PERM_READ | PERM_EXEC) == perms
    assert exit_code == SUCCESS

@pytest.mark.parametrize("value", [-1, 0, 1])
@pytest.mark.parametrize("param_format", [0, 1, 2])
def test_opt_integer(value, param_format):
    if param_format == 0:
        params = f"-i{value}"
    elif param_format == 1:
        params = f"--int {value}"
    elif param_format == 2:
        params = f"--int={value}"
    
    stdout, _, exit_code = run("basic", [params])
    out_value = int(json.loads(stdout)["int"])
    assert value == out_value
    assert exit_code == SUCCESS

@pytest.mark.parametrize("value", [-1.5, 0.0, 1.9])
@pytest.mark.parametrize("param_format", [0, 1, 2])
def test_opt_float(value, param_format):
    if param_format == 0:
        params = f"-s{value}"
    elif param_format == 1:
        params = f"--float {value}"
    elif param_format == 2:
        params = f"--float={value}"
    
    stdout, _, exit_code = run("basic", [params])
    out_value = float(json.loads(stdout)["float"]) 
    assert value == out_value
    assert exit_code == SUCCESS
    
@pytest.mark.parametrize("value", ["arg-parse215125"])
@pytest.mark.parametrize("param_format", [0, 1, 2])
def test_opt_string(value, param_format):
    if param_format == 0:
        params = f"-p{value}"
    elif param_format == 1:
        params = f"--path {value}"
    elif param_format == 2:
        params = f"--path={value}"
    
    stdout, _, exit_code = run("basic", [params])
    try:
        out_value = json.loads(stdout)["path"]
    except:
        raise ValueError(stdout)
    assert value == out_value
    assert exit_code == SUCCESS