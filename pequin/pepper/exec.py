import sys, math, os, time
DEFAULT_TIMEOUT = -1

def help_message():
    print("Format:")
    print("      python3 exec.py <benchmark> <timeout>")
    print("Valid <benchmark>:")
    print("  --all: test all benchmarks")
    print("      0: Find Min")
    print("      1: Merging")
    print("      2: Binary Search")
    print("      3: KMP Search")
    print("      4: Next Permutation")
    print("      5: Dutch Flag")
    print("      6: Recurrence Relations Sequence")
    print("      7: Sum of Powers")
    print("      8: 2D Convex Hull")
    print("      9: MSC")
    print("     10: MSC")
    print("Valid <timeout>: a positive number (none = generate the data used to produce the graph + 240 seconds timeout)")

def pequin_test(name, new_code, prefix, to, suffix = ""):
    global rec_file
    new_file = open("apps/" + name + ".c", "w")
    new_file.write(new_code)
    new_file.close()
    to = 300 if to == -1 else to
    output = os.popen("timeout " + str(to) + " bash test.sh " + name + " 2> log | grep --color=never 'NUMBER' | sed 's/\:/\ =/g'") 
    outr = output.read()
    print(prefix + outr[:-1], end = ""),
    if outr == "":
        print("Timeout after " + str(to) + " seconds.")
        return True
    else:
        print(suffix)
        rec_file.write(prefix + outr[25:])
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
if not param in ["--all", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"]:
    help_message()
    quit()

to = -1
if len(sys.argv) == 3:
    if sys.argv[2].isdigit() and int(sys.argv[2]) > 0:
        to = int(sys.argv[2])
    else:
        print("Invalid timeout number: must be a positive number.")
        quit()

# Set Up Test File
rec_file = open("result", "w")

# --
# 0 - Find Min
# Last test case for default is N = 5120
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "0"]:
    DEFAULT_LAST_N = 5120
    print("\n--\nTesting Benchmark 0: Find Min")
    rec_file.write("Benchmark 0\n")

    print("\nT_I: (N = length of array)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/find_min_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("find_min_ti", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_S: (N = length of array)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/find_min_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("find_min_ts", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/find_min_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("find_min_te", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

# --
# 1 - Merging
# Testing this example linearly
# Last test case for default is N = 10, L = 14
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "1"]:
    DEFAULT_LAST_N = 10
    DEFAULT_LAST_L = 14
    print("\n--\nTesting Benchmark 1: Merging")
    rec_file.write("Benchmark 1\n")

    print("\nT_I: (N = length of the longest array, L = number of arrays)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/merging_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        l = 2
        while not to_switch and not short_circuit:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_L " + str(l) + "\n" + sk_code
            to_switch = pequin_test("merging_ti", new_code, "N = " + str(n) + ", L = " + str(l) + ": ", to)
            if to == -1:
                short_circuit = n == DEFAULT_LAST_N and l == DEFAULT_LAST_L
            l += 2
            if not to_switch:
                to_success += 1
        n *= 2
    max_l = l

    print("\nT_S: (N = length of the longest array, L = number of arrays)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/merging_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        l = 2
        while not to_switch and l < max_l and not short_circuit:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_L " + str(l) + "\n" + sk_code
            to_switch = pequin_test("merging_ts", new_code, "N = " + str(n) + ", L = " + str(l) + ": ", to)
            if to == -1:
                short_circuit = n == DEFAULT_LAST_N and l == DEFAULT_LAST_L
            l += 2
            if not to_switch:
                to_success += 1
        n *= 2

    print("\nT_E: (N = length of the longest array, L = number of arrays)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/merging_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        l = 2
        while not to_switch and l < max_l and not short_circuit:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_L " + str(l) + "\n" + sk_code
            to_switch = pequin_test("merging_te", new_code, "N = " + str(n) + ", L = " + str(l) + ": ", to)
            if to == -1:
                short_circuit = n == DEFAULT_LAST_N and l == DEFAULT_LAST_L
            l += 2
            if not to_switch:
                to_success += 1
        n *= 2

# --
# 2 - Binary Search
# Last test case for default is N = 5120
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "2"]:
    DEFAULT_LAST_N = 5120

    print("\n--\nTesting Benchmark 2: Binary Search")
    rec_file.write("Benchmark 2\n")

    print("\nT_I: (N = length of array)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/binary_search_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        l = int(math.ceil(math.log(n, 2)))
        new_code = "#define MAX_N " + str(n) + "\n#define MAX_LOG " + str(l) + "\n" + sk_code
        to_switch = pequin_test("binary_search_ti", new_code, "N = " + str(n) + ", LOG_N = " + str(l) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_S: (N = length of array)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/binary_search_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("binary_search_ts", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/binary_search_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("binary_search_te", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nFor the following 2 examples, set limit to N = 160.")
    print("T_I, without RAM Initialization: (N = length of array)")
    rec_file.write("T_IA\n")
    sk_file = open(r"skeletons/binary_search_ti_no_init.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch and n <= 160:
        l = int(math.ceil(math.log(n, 2)))
        new_code = "#define MAX_N " + str(n) + "\n#define MAX_LOG " + str(l) + "\n" + sk_code
        to_switch = pequin_test("binary_search_ti_no_init", new_code, "N = " + str(n) + ", LOG_N = " + str(l) + ": ", to)
        n *= 2

    n_term = n

    print("\nT_E, without RAM Initialization: (N = length of array)")
    rec_file.write("T_BA\n")
    sk_file = open(r"skeletons/binary_search_te_no_init.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    # Execute it at most 4 times to show it is constant
    while not to_switch and n < n_term:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("binary_search_te_no_init", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 3 - KMP Search
# Last test case for default is N = 320, M = 128
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "3"]:
    DEFAULT_LAST_N = 320
    DEFAULT_LAST_M = 128

    print("\n--\nTesting Benchmark 3: KMP Search")
    rec_file.write("Benchmark 3\n")

    print("\nT_I: (N = length of text, M = length of pattern)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/kmp_search_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        m = 2
        while (not to_switch) and n > m * 2 and not short_circuit:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("kmp_search_ti", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            if to == -1:
                short_circuit = n == DEFAULT_LAST_N and m == DEFAULT_LAST_M
            m *= 2
            if not to_switch:
                to_success += 1
        n *= 2

    print("\nT_S & T_E: (N = length of text, M = length of pattern)")
    rec_file.write("T_SB\n")
    sk_file = open(r"skeletons/kmp_search_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        m = 2
        while (not to_switch) and n > m * 2 and not short_circuit:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("kmp_search_te", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            if to == -1:
                short_circuit = n == DEFAULT_LAST_N and m == DEFAULT_LAST_M
            m *= 2
            if not to_switch:
                to_success += 1
        n *= 2

# --
# 4 - Next Permutation
# Last test case for default is N = 1280
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "4"]:
    DEFAULT_LAST_N = 1280

    print("\n--\nTesting Benchmark 4: Next Permutation")
    rec_file.write("Benchmark 4\n")

    print("\nT_I: (N = length of array)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/next_permutation_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("next_permutation_ti", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_S: (N = length of array)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/next_permutation_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        l = math.factorial(n)
        new_code = "#define MAX_N " + str(n) + "\n#define MAX_FAC " + str(l) + "\n" + sk_code
        to_switch = pequin_test("next_permutation_ts", new_code, "N = " + str(n) + ", FAC_N = " + str(l) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/next_permutation_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("next_permutation_te", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

# --
# 5 - Dutch Flag
# Last test case for default is N = 320
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "5"]:
    DEFAULT_LAST_N = 320

    print("\n--\nTesting Benchmark 5: Dutch Flag")
    rec_file.write("Benchmark 5\n")

    print("\nT_I: (N = length of array)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/dutch_flag_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("dutch_flag_ti", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_S: (N = length of array)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/dutch_flag_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("dutch_flag_ts", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/dutch_flag_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("dutch_flag_te", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

# --
# 6 - Recurrence Relations Sequence
# Last test case for default is N = 10, M = 128
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "6"]:
    DEFAULT_LAST_N = 80
    DEFAULT_LAST_M = 16

    print("\n--\nTesting Benchmark 6: Recurrence Relations Sequence")
    rec_file.write("Benchmark 6\n")

    print("\nT_I: (N = length of the sequence, M = number of recurrence relations)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/rr_sequence_find_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        m = 2
        while not to_switch and not short_circuit:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("rr_sequence_find_ti", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            if to == -1:
                short_circuit = n == DEFAULT_LAST_N and m == DEFAULT_LAST_M
            m *= 2
            if not to_switch:
                to_success += 1
        n *= 2

    print("\nT_S: (N = number of elements in the sequence, M = number of recurrence relations)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/rr_sequence_find_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        m = 2
        while not to_switch and not short_circuit:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("rr_sequence_find_ts", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            if to == -1:
                short_circuit = n == DEFAULT_LAST_N and m == DEFAULT_LAST_M
            m *= 2
            if not to_switch:
                to_success += 1
        n *= 2

    print("\nT_E: (N = number of elements in the sequence, M = number of recurrence relations)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/rr_sequence_find_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        m = 2
        while not to_switch and not short_circuit:
            new_code = "#define MAX_N " + str(n) + "\n" + "#define M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("rr_sequence_find_te", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            if to == -1:
                short_circuit = n == DEFAULT_LAST_N and m == DEFAULT_LAST_M
            m *= 2
            if not to_switch:
                to_success += 1
        n *= 2

# --
# 7 - Sum of Powers
# Last test case for default is R = 320, K = 4
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "7"]:
    DEFAULT_LAST_R = 320
    DEFAULT_LAST_K = 4

    print("\n--\nTesting Benchmark 7: Sum of Powers")
    rec_file.write("Benchmark 7\n")

    print("\nT_I: (Want X^K + Y^K = R, X > Y)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/sum_of_powers_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    r = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        k = 1
        while (not to_switch) and 2 ** k < r and not short_circuit:
            new_code = "#define MAX_R " + str(r) + "\n" + "#define MAX_K " + str(k) + "\n" + sk_code
            to_switch = pequin_test("sum_of_powers_ti", new_code, "R = " + str(r) + ", K = " + str(k) + ": ", to)
            if to == -1:
                short_circuit = r == DEFAULT_LAST_R and k == DEFAULT_LAST_K
            k *= 2
            if not to_switch:
                to_success += 1
        r *= 2

    print("\nT_S: (Want X^K + Y^K = R, X > Y)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/sum_of_powers_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    r = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        k = 1
        while (not to_switch) and 2 ** k < r and not short_circuit:
            new_code = "#define MAX_R " + str(r) + "\n" + "#define MAX_K " + str(k) + "\n" + sk_code
            to_switch = pequin_test("sum_of_powers_ts", new_code, "R = " + str(r) + ", K = " + str(k) + ": ", to)
            if to == -1:
                short_circuit = r == DEFAULT_LAST_R and k == DEFAULT_LAST_K
            k *= 2
            if not to_switch:
                to_success += 1
        r *= 2

    print("\nT_E: (Want X^K + Y^K = R, X > Y)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/sum_of_powers_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    r = 10
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        k = 1
        while (not to_switch) and 2 ** k < r and not short_circuit:
            new_code = "#define MAX_R " + str(r) + "\n" + "#define MAX_K " + str(k) + "\n" + sk_code
            to_switch = pequin_test("sum_of_powers_te", new_code, "R = " + str(r) + ", K = " + str(k) + ": ", to)
            if to == -1:
                short_circuit = r == DEFAULT_LAST_R and k == DEFAULT_LAST_K
            k *= 2
            if not to_switch:
                to_success += 1
        r *= 2

# --
# 8 - 2D Convex Hull
# Last test case for default is N = 160
# We also enforce a 300 seconds timeout for default case
if param in ["--all", "8"]:
    DEFAULT_LAST_N = 160

    print("\n--\nTesting Benchmark 8: 2D Convex Hull")
    rec_file.write("Benchmark 8\n")

    print("\nT_I: (N = length of array)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/td_convex_hull_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_ti", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_S: (N = length of array)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/td_convex_hull_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_ts", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/td_convex_hull_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_te", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

    print("\nT_E, Annotation Only: (N = length of array)")
    rec_file.write("T_BA\n")
    sk_file = open(r"skeletons/td_convex_hull_te_ann.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_te_ann", new_code, "N = " + str(n) + ": ", to)
        if to == -1:
            to_switch = to_switch or n == DEFAULT_LAST_N
        n *= 2

# --
# 9 - MSC
# Last test case for default is V = 20, E = 400
# We also enforce a 2000 seconds timeout for default case
if param in ["--all", "9"]:
    DEFAULT_LAST_V = 20
    DEFAULT_LAST_E = 400

    print("\n--\nTesting Benchmark 9: MSC")
    rec_file.write("Benchmark 9\n")
    
    print("\nT_I: (V = number of nodes, E = number of edges)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/msc_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= v ** 2 and not short_circuit:
            new_code = "#define MAX_V " + str(v) + "\n" + "#define MAX_E " + str(e) + "\n" + sk_code
            to_switch = pequin_test("msc_ti", new_code, "V = " + str(v) + ", E = " + str(e) + ": ", 2000 if to == -1 else to)
            if to == -1:
                short_circuit = v == DEFAULT_LAST_V and e == DEFAULT_LAST_E
            if e == v ** 2:
                to_switch = True
            e *= 2
            if e > v ** 2:
                e = v ** 2
            if not to_switch:
                to_success += 1
        v *= 2
    max_v = v

    print("\nT_S: (V = number of nodes, E = number of edges)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/msc_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    exp = 2 ** v
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= v ** 2 and not short_circuit:
            new_code = "#define MAX_V " + str(v) + "\n" + "#define MAX_E " + str(e) + "\n" + "#define MAX_EXP " + str(exp) + "\n" + sk_code
            to_switch = pequin_test("msc_ts", new_code, "V = " + str(v) + ", E = " + str(e) + ", EXP_V = " + str(exp) + ": ", 2000 if to == -1 else to)
            if to == -1:
                short_circuit = v == DEFAULT_LAST_V and e == DEFAULT_LAST_E
            if e == v ** 2:
                to_switch = True
            e *= 2
            if e > v ** 2:
                e = v ** 2
            if not to_switch:
                to_success += 1
        v *= 2
        exp = 2 ** v
        # Pequin cannot handle 2^20 iterations, so we timeout by default
        if v == 20:
            short_circuit = True

    print("\nT_E: (V = number of nodes, E = number of edges)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/msc_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    to_success = -1
    short_circuit = False
    while to_success != 0 and v < max_v and not short_circuit:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= v ** 2 and not short_circuit:
            new_code = "#define MAX_V " + str(v) + "\n" + "#define MAX_E " + str(e) + "\n" + sk_code
            to_switch = pequin_test("msc_te", new_code, "V = " + str(v) + ", E = " + str(e) + ": ", 2000 if to == -1 else to)
            if to == -1:
                short_circuit = v == DEFAULT_LAST_V and e == DEFAULT_LAST_E
            if e == v ** 2:
                to_switch = True
            e *= 2
            if e > v ** 2:
                e = v ** 2
            if not to_switch:
                to_success += 1
        v *= 2

# --
# 10 - MST
# Last test case for default is V = 15, E = 30
# We also enforce a 2000 seconds timeout for default case
if param in ["--all", "10"]:
    DEFAULT_LAST_V = 15
    DEFAULT_LAST_E = 30

    print("\n--\nTesting Benchmark 10: MST")
    rec_file.write("Benchmark 10\n")
    
    print("\nT_I: (V = number of nodes, E = number of edges)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/mst_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    to_success = -1
    short_circuit = False
    while to_success != 0 and not short_circuit:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= DEFAULT_LAST_E and not short_circuit:
            l = int(math.ceil(math.log(v, 2)))
            extra_cons = 22 * (e * 3 * int(math.ceil(math.log(e * 3, 2))) - e * 3 + 1)
            extra_cons += 7 * (e * int(math.ceil(math.log(e, 2))) - e + 1)
            new_code = f"#define MAX_VERTICES {v}\n#define INV_ACKER_EDGES 3\n#define MAX_EDGES {e}\n#define LOG_VERTICES {l}\n" + sk_code
            to_switch = pequin_test("mst_ti", new_code, "V = " + str(v) + ", E = " + str(e) + ": ", 2000 if to == -1 else to, " + " + str(extra_cons))
            if to == -1:
                short_circuit = v == DEFAULT_LAST_V and e == DEFAULT_LAST_E
            to_switch = to_switch or e == DEFAULT_LAST_E
            e += 10
            if not to_switch:
                to_success += 1
        v += 5
    max_v = v

    print("\nT_S: (V = number of nodes, E = number of edges)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/mst_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    exp = 2 ** v
    to_success = -1
    short_circuit = False
    while to_success != 0 and v < max_v and not short_circuit:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= DEFAULT_LAST_E and not short_circuit:
            l = int(math.ceil(math.log(v, 2)))
            new_code = f"#define MAX_VERTICES {v}\n#define INV_ACKER_EDGES 3\n#define MAX_EDGES {e}\n#define LOG_VERTICES {l}\n" + sk_code
            to_switch = pequin_test("mst_ts", new_code, "V = " + str(v) + ", E = " + str(e) + ", EXP_V = " + str(exp) + ": ", 2000 if to == -1 else to)
            if to == -1:
                short_circuit = v == DEFAULT_LAST_V and e == DEFAULT_LAST_E
            to_switch = to_switch or e == DEFAULT_LAST_E
            e += 10
            if not to_switch:
                to_success += 1
        v += 5
        exp = 2 ** v

    print("\nT_E: (V = number of nodes, E = number of edges)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/mst_te.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    to_success = -1
    short_circuit = False
    while to_success != 0 and v < max_v and not short_circuit:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= DEFAULT_LAST_E and not short_circuit:
            l = int(math.ceil(math.log(v, 2)))
            extra_cons = 22 * (e * 3 * int(math.ceil(math.log(e * 3, 2))) - e * 3 + 1)
            extra_cons += 7 * (e - v + 1 + e * int(math.ceil(math.log(v - 1, 4))))
            new_code = f"#define MAX_VERTICES {v}\n#define INV_ACKER_EDGES 3\n#define MAX_EDGES {e}\n#define LOG_VERTICES {l}\n" + sk_code
            to_switch = pequin_test("mst_te", new_code, "V = " + str(v) + ", E = " + str(e) + ": ", 2000 if to == -1 else to, " + " + str(extra_cons))
            if to == -1:
                short_circuit = v == DEFAULT_LAST_V and e == DEFAULT_LAST_E
            to_switch = to_switch or e == DEFAULT_LAST_E
            e += 10
            if not to_switch:
                to_success += 1
        v += 5

rec_file.write("End\n")
rec_file.close()
print("\nPlease refer to $PEQUIN/pepper/log for any stderr outputs.")
