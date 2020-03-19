import os.path
import subprocess

from interp import Interpreter

tests = [
    ("add", 3),
    ("sub_vars_declared_global", 1),
    ("single_if", 42),
    ("if_else", 23),
    ("func_simple", 5),
    ("fact5", 120),
]
testDir = "tests"
testExtension = ".c"
compiler_output_filename = "out.tac"

def run_test(filename):
    print('Running test: {}'.format(filename))
    full_name = os.path.join(testDir, filename)
    full_name = full_name + testExtension

    with open(compiler_output_filename, 'w') as tac_file, open(full_name) as c_file:
        subprocess.run(["./compiler"], stdout=tac_file, stdin=c_file)

    with open(compiler_output_filename) as tac_file:
        tac = tac_file.read()

    res = Interpreter.eval(tac)
    return res

if __name__ == "__main__":
    failed = []
    for (filename, expected) in tests:
        res = run_test(filename)
        if res != expected:
            failed.append((filename, expected, res))

    if len(failed) > 0:
        print('{} of {} tests failed: {}'.format(len(failed), len(tests), failed))
        for filename, expected, res in failed:
            print("{}: expected {}, got {}".format(filename, expected, res))
    else:
        print('All tests passed!')
