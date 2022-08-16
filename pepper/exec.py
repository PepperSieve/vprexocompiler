import sys, math, os, time
DEFAULT_TIMEOUT = 100

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
    print("Valid <timeout>: a positive number (default = " + str(DEFAULT_TIMEOUT) + " seconds)")

def pequin_test(name, new_code, prefix, to):
    global rec_file
    new_file = open("apps/" + name + ".c", "w")
    new_file.write(new_code)
    new_file.close()
    output = os.popen("timeout " + str(to) + " bash test.sh " + name + " 2> log | grep --color=never 'NUMBER' | sed 's/\:/\ =/g'") 
    outr = output.read()
    print(prefix + outr),
    if outr == "":
        print("Timeout after " + str(to) + " seconds.")
        return True
    else:
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
if not param in ["--all", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]:
    help_message()
    quit()

to = DEFAULT_TIMEOUT
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
if param in ["--all", "0"]:
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
        new_code = "#define MAX_N " + str(n) + sk_code
        to_switch = pequin_test("find_min_ti", new_code, "N = " + str(n) + ": ", to)
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
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/find_min_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("find_min_tb", new_code, "N = " + str(n) + ": ", to)
        n *= 2

# --
# 1 - Merging
if param in ["--all", "1"]:
    print("\n--\nTesting Benchmark 1: Merging")
    rec_file.write("Benchmark 1\n")

    print("\nT_I: (N = length of the longest array, L = number of arrays)")
    rec_file.write("T_I\n")
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
    rec_file.write("T_S\n")
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

    print("\nT_E: (N = length of the longest array, L = number of arrays)")
    rec_file.write("T_B\n")
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
# 2 - Binary Search
if param in ["--all", "2"]:
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
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
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
# 3 - KMP Search
if param in ["--all", "3"]:
    print("\n--\nTesting Benchmark 3: KMP Search")
    rec_file.write("Benchmark 3\n")

    print("\nT_I: (N = length of text, M = length of pattern)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/kmp_search_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    while to_success != 1:
        to_success = 0
        to_switch = False
        m = 2
        while (not to_switch) and n > m * 2:
            to_success += 1
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("kmp_search_ti", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            m *= 2
        n *= 2

    print("\nT_S & T_E: (N = length of text, M = length of pattern)")
    rec_file.write("T_SB\n")
    sk_file = open(r"skeletons/kmp_search_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    n = 10
    to_success = -1
    while to_success != 1:
        to_success = 0
        to_switch = False
        m = 2
        while (not to_switch) and n > m * 2:
            to_success += 1
            new_code = "#define MAX_N " + str(n) + "\n" + "#define MAX_M " + str(m) + "\n" + sk_code
            to_switch = pequin_test("kmp_search_tb", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            m *= 2
        n *= 2

# --
# 4 - Next Permutation
if param in ["--all", "4"]:
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
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
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
# 5 - Dutch Flag
if param in ["--all", "5"]:
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
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
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
# 6 - Recurrence Relations Sequence
if param in ["--all", "6"]:
    print("\n--\nTesting Benchmark 6: Recurrence Relations Sequence")
    rec_file.write("Benchmark 6\n")

    print("\nT_I: (N = length of the sequence, M = number of recurrence relations)")
    rec_file.write("T_I\n")
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

    print("\nT_S: (N = number of elements in the sequence, M = number of recurrence relations)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/rr_sequence_find_ts.c", "r")
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
            to_switch = pequin_test("rr_sequence_find_ts", new_code, "N = " + str(n) + ", M = " + str(m) + ": ", to)
            m *= 2
            if not to_switch:
                to_success += 1
        n *= 2

    print("\nT_E: (N = number of elements in the sequence, M = number of recurrence relations)")
    rec_file.write("T_B\n")
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
# 7 - Sum of Powers
if param in ["--all", "7"]:
    print("\n--\nTesting Benchmark 7: Sum of Powers")
    rec_file.write("Benchmark 7\n")

    print("\nT_I: (Want X^K + Y^K = R, X > Y)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/sum_of_powers_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    r = 10
    to_success = -1
    while to_success != 1:
        to_success = 0
        to_switch = False
        k = 1
        while (not to_switch) and 2 ** k < r:
            to_success += 1
            new_code = "#define MAX_R " + str(r) + "\n" + "#define MAX_K " + str(k) + "\n" + sk_code
            to_switch = pequin_test("sum_of_powers_ti", new_code, "R = " + str(r) + ", K = " + str(k) + ": ", to)
            k *= 2
        r *= 2

    print("\nT_S: (Want X^K + Y^K = R, X > Y)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/sum_of_powers_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    r = 10
    to_success = -1
    while to_success != 1:
        to_success = 0
        to_switch = False
        k = 1
        while (not to_switch) and 2 ** k < r:
            to_success += 1
            new_code = "#define MAX_R " + str(r) + "\n" + "#define MAX_K " + str(k) + "\n" + sk_code
            to_switch = pequin_test("sum_of_powers_ts", new_code, "R = " + str(r) + ", K = " + str(k) + ": ", to)
            k *= 2
        r *= 2

    print("\nT_E: (Want X^K + Y^K = R, X > Y)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/sum_of_powers_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    r = 10
    to_success = -1
    while to_success != 1:
        to_success = 0
        to_switch = False
        k = 1
        while (not to_switch) and 2 ** k < r:
            to_success += 1
            new_code = "#define MAX_R " + str(r) + "\n" + "#define MAX_K " + str(k) + "\n" + sk_code
            to_switch = pequin_test("sum_of_powers_tb", new_code, "R = " + str(r) + ", K = " + str(k) + ": ", to)
            k *= 2
        r *= 2

# --
# 8 - 2D Convex Hull
if param in ["--all", "8"]:
    print("\n--\nTesting Benchmark 8: 2D Convex Hull")
    rec_file.write("Benchmark 8\n")

    print("\nT_I: (N = length of array)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/2d_convex_hull_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_ti", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nT_S: (N = length of array)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/2d_convex_hull_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_ts", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nT_E: (N = length of array)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/2d_convex_hull_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    to_switch = False
    n = 10
    while not to_switch:
        new_code = "#define MAX_N " + str(n) + "\n" + sk_code
        to_switch = pequin_test("td_convex_hull_tb", new_code, "N = " + str(n) + ": ", to)
        n *= 2

    print("\nT_E, Annotation Only: (N = length of array)")
    rec_file.write("T_BA\n")
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
# 9 - MSC
if param in ["--all", "9"]:
    print("\n--\nTesting Benchmark 9: MSC")
    rec_file.write("Benchmark 9\n")

    print("\nT_I: (V = number of nodes, E = number of edges)")
    rec_file.write("T_I\n")
    sk_file = open(r"skeletons/msc_ti.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    to_success = -1
    while to_success != 1:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= v ** 2:
            to_success += 1
            new_code = "#define MAX_V " + str(v) + "\n" + "#define MAX_E " + str(e) + "\n" + sk_code
            to_switch = pequin_test("msc_ti", new_code, "V = " + str(v) + ", E = " + str(e) + ": ", to)
            if e == v ** 2:
                to_switch = True
            e *= 2
            if e > v ** 2:
                e = v ** 2
        v *= 2

    print("\nT_S: (V = number of nodes, E = number of edges)")
    rec_file.write("T_S\n")
    sk_file = open(r"skeletons/msc_ts.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    exp = 2 ** v
    to_success = -1
    while to_success != 1:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= v ** 2:
            to_success += 1
            new_code = "#define MAX_V " + str(v) + "\n" + "#define MAX_E " + str(e) + "\n" + "#define MAX_EXP " + str(exp) + "\n" + sk_code
            to_switch = pequin_test("msc_ts", new_code, "V = " + str(v) + ", E = " + str(e) + ", EXP_V = " + str(exp) + ": ", to)
            if e == v ** 2:
                to_switch = True
            e *= 2
            if e > v ** 2:
                e = v ** 2
        v *= 2
        exp = 2 ** v

    print("\nT_E: (V = number of nodes, E = number of edges)")
    rec_file.write("T_B\n")
    sk_file = open(r"skeletons/msc_tb.c", "r")
    sk_code = sk_file.read()
    sk_file.close()
    v = 5
    to_success = -1
    while to_success != 1:
        to_success = 0
        to_switch = False
        e = 10
        while (not to_switch) and e <= v ** 2:
            to_success += 1
            new_code = "#define MAX_V " + str(v) + "\n" + "#define MAX_E " + str(e) + "\n" + sk_code
            to_switch = pequin_test("msc_tb", new_code, "V = " + str(v) + ", E = " + str(e) + ": ", to)
            if e == v ** 2:
                to_switch = True
            e *= 2
            if e > v ** 2:
                e = v ** 2
        v *= 2

rec_file.write("End\n")
rec_file.close()
print("\nPlease refer to $PEQUIN/pepper/log for any stderr outputs.")
