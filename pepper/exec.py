import sys, math, os, time

def help_message():
    print("format: python3 exec.py param timeout")
    print("Valid param")
    print("  --all: test all benchmarks")
    print("      0: Merging")
    print("      1: Binary Search")
    print("      2: Next Permutation")
    print("      3: Dutch Flag")
    print("      4: Recurrence Relations Sequence")
    print("      5: Sum of Powers")
    print("      6: 2D Convex Hull")
    print("      7: MSC")
    print("Valid timeout: a positive number (default = 600 seconds)")

def pequin_test(name, new_code, prefix, to):
    new_file = open("apps/" + name + ".c", "w")
    new_file.write(new_code)
    new_file.close()
    output = os.popen("timeout " + str(to) + " bash test.sh " + name + " 2> log | grep --color=never 'NUMBER' | sed 's/\:/\ =/g'") 
    outr = output.read()
    print(prefix + outr, end = "")
    if outr == "":
        print("Timeout after " + str(to) + " seconds.")
        return True
    return False

# --
# Main
# --

# --
# Preliminary
if not len(sys.argv) in [2, 3]:
    print("Invalid input: Wrong parameter number.")
    help_message()
    quit()
param = sys.argv[1]
if not param in ["--all", "0", "1", "2", "3", "4", "5", "6", "7"]:
    help_message()
    quit()

to = 600
if len(sys.argv) == 3:
    if sys.argv[2].isdigit() and int(sys.argv[2]) > 0:
        to = int(sys.argv[2])
    else:
        print("Invalid timeout number: must be a positive number.")
        quit()

# --
# 0 - Merging

# --
# 1 - Binary Search
if param in ["--all", "1"]:
    print("\nTesting Benchmark 1: Binary Search")

    print("--\nOriginal Method, T_I: (N = length of array)")
    sk_file = open(r"skeletons/binary_search_sk.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        l = math.ceil(math.log(n, 2))
        new_code = "#define MAX_N " + str(n) + "\n#define MAX_LOG " + str(l) + "\n" + sk_code
        to_switch = pequin_test("binary_search", new_code, "N = " + str(n) + ", LOG_N = " + str(l) + ": ", to)
        n *= 2

    print("\nSpecification Method, T_B: (N = length of array)")
    sk_file = open(r"skeletons/binary_search_sp_sk.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("binary_search_sp", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 2 - Next Permutation
if param in ["--all", "2"]:
    print("--\nTesting Benchmark 2: Next Permutation")

    print("\nOriginal Method, T_I: (N = length of array)")
    sk_file = open(r"skeletons/next_permutation_sk.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("next_permutation", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nSpecification Method, T_B: (N = length of array)")
    sk_file = open(r"skeletons/next_permutation_sp_sk.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("next_permutation_sp", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 3 - Dutch Flag
if param in ["--all", "3"]:
    print("--\nTesting Benchmark 3: Dutch Flag")

    print("\nOriginal Method, T_I: (N = length of array)")
    sk_file = open(r"skeletons/dutch_flag_sk.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("dutch_flag", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nSpecification Method, T_B: (N = length of array)")
    sk_file = open(r"skeletons/dutch_flag_sp_sk.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("dutch_flag_sp", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 4 - Recurrence Relations Sequence

# --
# 5 - Sum of Powers

print("\nPlease refer to ./log for any stderr outputs.")
    