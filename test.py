import os.path
import subprocess

tests = [["return1", 1], ["one_plus_one", 2], ["three_times_four", 12], ["sum_ten", 55], ["fact5", 120], ["jump1", 5]]
testDir = "test"
testExtension = ".tac"
testAsm = "testAsm.s"
testExec = "testExec"

def run_test(filename, expected):
    print('Running test: {}'.format(filename))
    full_name = os.path.join(testDir, filename)
    full_name = full_name + testExtension

    with open(testAsm, 'w') as asm_file, open(full_name) as tac_file:
        subprocess.run(["./compiler", full_name], stdout=asm_file, stdin=tac_file)

    subprocess.run(["gcc", "-o", testExec, testAsm])
    procResult = subprocess.run("./" + testExec)
    returnCode = procResult.returncode
    return returnCode == expected

if __name__ == "__main__":
    failed = []
    for (filename, expected) in tests:
        res = run_test(filename, expected)
        if not res:
            failed.append(filename)

    if len(failed) > 0:
        print('{} of {} tests failed: {}'.format(len(failed), len(tests), failed))
    else:
        print('All tests passed!')
