import sys, math, os, time
DEFAULT_TIMEOUT = 100

def help_message():
    print("Format:")
    print("      python3 exec.py param timeout")
    print("Valid param:")
    print("  --all: test all benchmarks")
    print("      0: Merging")
    print("      1: Binary Search")
    print("      2: Next Permutation")
    print("      3: Dutch Flag")
    print("      4: Recurrence Relations Sequence")
    print("      5: Sum of Powers")
    print("      6: 2D Convex Hull")
    print("      7: MSC")
    print("Valid timeout: a positive number (default = " + DEFAULT_TIMEOUT + " seconds)")

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

to = DEFAULT_TIMEOUT
if len(sys.argv) == 3:
    if sys.argv[2].isdigit() and int(sys.argv[2]) > 0:
        to = int(sys.argv[2])
    else:
        print("Invalid timeout number: must be a positive number.")
        quit()

# --
# 0 - Merging
if param in ["--all", "0"]:
    print("\n--\nTesting Benchmark 0: Merging")

    print("\nT_I: (N = length of the longest array, L = number of arrays)")
    sk_file = open(r"skeletons/merging_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        l = 2
        while not to_switch:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_L " + str(l) + "\n" + sk_code
            to_switch = pequin_test("merging_ti", new_code, "N = " + str(n) + ", L = " + str(l) + ": ", to)
            l *= 2
            if not to_switch:
                to_success += 1
        n *= 2

    print("\nT_S: (N = length of the longest array, L = number of arrays)")
    sk_file = open(r"skeletons/merging_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        l = 2
        while not to_switch:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_L " + str(l) + "\n" + sk_code
            to_switch = pequin_test("merging_ts", new_code, "N = " + str(n) + ", L = " + str(l) + ": ", to)
            l *= 2
            if not to_switch:
                to_success += 1
        n *= 2

    print("\nT_B: (N = length of the longest array, L = number of arrays)")
    sk_file = open(r"skeletons/merging_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        l = 2
        while not to_switch:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_L " + str(l) + "\n" + sk_code
            to_switch = pequin_test("merging_tb", new_code, "N = " + str(n) + ", L = " + str(l) + ": ", to)
            l *= 2
            if not to_switch:
                to_success += 1
        n *= 2

# --
# 1 - Binary Search
if param in ["--all", "1"]:
    print("\n--\nTesting Benchmark 1: Binary Search")

    print("\nT_I: (N = length of array)")
    sk_file = open(r"skeletons/binary_search_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        l = math.ceil(math.log(n, 2))
        new_code = "#define MAX_N " + str(n) + "\n#define MAX_LOG " + str(l) + "\n" + sk_code
        to_switch = pequin_test("binary_search_ti", new_code, "N = " + str(n) + ", LOG_N = " + str(l) + ": ", to)
        n *= 2

    print("\nT_B: (N = length of array)")
    sk_file = open(r"skeletons/binary_search_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("binary_search_tb", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 2 - Next Permutation
if param in ["--all", "2"]:
    print("\n--\nTesting Benchmark 2: Next Permutation")

    print("\nT_I: (N = length of array)")
    sk_file = open(r"skeletons/next_permutation_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("next_permutation_ti", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nT_B: (N = length of array)")
    sk_file = open(r"skeletons/next_permutation_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("next_permutation_tb", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 3 - Dutch Flag
if param in ["--all", "3"]:
    print("\n--\nTesting Benchmark 3: Dutch Flag")

    print("\nT_I: (N = length of array)")
    sk_file = open(r"skeletons/dutch_flag_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("dutch_flag_ti", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nT_B: (N = length of array)")
    sk_file = open(r"skeletons/dutch_flag_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("dutch_flag_tb", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 4 - Recurrence Relations Sequence
if param in ["--all", "4"]:
    print("\n--\nTesting Benchmark 4: Recurrence Relations Sequence")

    print("\nT_I: (N = length of the sequence, M = number of recurrence relations)")
    sk_file = open(r"skeletons/rr_sequence_find_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        m = 2
        while not to_switch:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("rr_sequence_find_ti", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            m *= 2
            if not to_switch:
                to_success += 1
        n *= 2

    print("\nT_B: (N = number of elements in the sequence, M = number of recurrence relations)")
    sk_file = open(r"skeletons/rr_sequence_find_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        m = 2
        while not to_switch:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("rr_sequence_find_tb", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            m *= 2
            if not to_switch:
                to_success += 1
        n *= 2

# --
# 5 - Sum of Powers
if param in ["--all", "5"]:
    print("\n--\nTesting Benchmark 5: Sum of Powers")

    print("\nT_I: (Want X^K + Y^K = R, X > Y)")
    sk_file = open(r"skeletons/sum_of_powers_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    r = 10
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        k = 1
        while (not to_switch) and 2 ** k < r:
            new_code = "#define MAX_R " + str(r) + "\n" + "#define MAX_K " + str(k) + "\n" + sk_code
            to_switch = pequin_test("sum_of_powers_ti", new_code, "R = " + str(r) + ", K = " + str(k) + ": ", to)
            k *= 2
            if not to_switch:
                to_success += 1
        r *= 2

    print("\nT_B: (Want X^K + Y^K = R, X > Y)")
    sk_file = open(r"skeletons/sum_of_powers_sp_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    r = 10
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        k = 1
        while (not to_switch) and k ** 2 <= r:
            new_code = "#define MAX_R " + str(r) + "\n" + "#define MAX_K " + str(k) + "\n" + sk_code
            to_switch = pequin_test("sum_of_powers_tb", new_code, "R = " + str(r) + ", K = " + str(k) + ": ", to)
            k *= 2
            if not to_switch:
                to_success += 1
        r *= 2

# --
# 6 - 2D Convex Hull
if param in ["--all", "6"]:
    print("\n--\nTesting Benchmark 6: 2D Convex Hull")

    print("\nT_I: (N = length of array)")
    sk_file = open(r"skeletons/2d_convex_hull_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_ti", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nT_B: (N = length of array)")
    sk_file = open(r"skeletons/2d_convex_hull_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_tb", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nT_B, Annotation Only: (N = length of array)")
    sk_file = open(r"skeletons/2d_convex_hull_tb_ann.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_tb_ann", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 7 - MSC
if param in ["--all", "7"]:
    print("\n--\nTesting Benchmark 7: MSC")

    print("\nT_I: (V = number of nodes, E = number of edges)")
    sk_file = open(r"skeletons/msc_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= v ** 2:
            new_code = "#define MAX_V " + str(v) + "\n" + "#define MAX_E " + str(e) + "\n" + sk_code
            to_switch = pequin_test("msc_ti", new_code, "V = " + str(v) + ", E = " + str(e) + ": ", to)
            if e == v ** 2:
                to_switch = True
            e *= 2
            if e > v ** 2:
                e = v ** 2
            if not to_switch:
                to_success += 1
        v *= 2

    print("\nT_B: (V = number of nodes, E = number of edges)")
    sk_file = open(r"skeletons/msc_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    to_success = -1
    while to_success != 0:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= v ** 2:
            new_code = "#define MAX_V " + str(v) + "\n" + "#define MAX_E " + str(e) + "\n" + sk_code
            to_switch = pequin_test("msc_tb", new_code, "V = " + str(v) + ", E = " + str(e) + ": ", to)
            if e == v ** 2:
                to_switch = True
            e *= 2
            if e > v ** 2:
                e = v ** 2
            if not to_switch:
                to_success += 1
        v *= 2

print("\nPlease refer to ./log for any stderr outputs.")
    