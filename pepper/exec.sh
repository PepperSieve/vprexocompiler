if [[ $# -eq 0 || $1 == 0 ]]; then
  echo -e "--\nTesting Benchmark 0: Merging" &&
  echo -e "\nOriginal Method, T2: (N = length of the longest array, L = number of arrays)" &&
  echo -n "N = 10, L = 2: " && bash test.sh merging_10_2 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 10, L = 5: " && bash test.sh merging_10_5 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 10, L = 10: " && bash test.sh merging_10_10 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, L = 5: " && bash test.sh merging_20_5 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, L = 10: " && bash test.sh merging_20_10 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\nNaive Method, T0: (N = length of the longest array, L = number of arrays)" &&
  echo -n "N = 10, L = 2: " && bash test.sh merging_10_2_t0 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 10, L = 5: " && bash test.sh merging_10_5_t0 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 10, L = 10: " && bash test.sh merging_10_10_t0 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, L = 5: " && bash test.sh merging_20_5_t0 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, L = 10: " && bash test.sh merging_20_10_t0 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\nSpecification Method, T1: (N = length of the longest array, L = number of arrays)" &&
  echo -n "N = 10, L = 2: " && bash test.sh merging_10_2_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 10, L = 5: " && bash test.sh merging_10_5_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 10, L = 10: " && bash test.sh merging_10_10_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, L = 5: " && bash test.sh merging_20_5_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, L = 10: " && bash test.sh merging_20_10_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\n"
fi &&

if [[ $# -eq 0 || $1 == 1 ]]; then
  echo -e "--\nTesting Benchmark 1: Binary Search" &&
  echo -e "\nOriginal Method, T2: (N = length of array)" &&
  echo -n "N = 10, LOG_N = 4: " && bash test.sh binary_search_10 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, LOG_N = 5: " && bash test.sh binary_search_20 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 30, LOG_N = 5: " && bash test.sh binary_search_30 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 40, LOG_N = 6: " && bash test.sh binary_search_40 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, LOG_N = 6: " && bash test.sh binary_search_50 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\nSpecification Method, T1: (N = length of array)" &&
  echo -n "N = 10, LOG_N = 4: " && bash test.sh binary_search_10_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, LOG_N = 5: " && bash test.sh binary_search_20_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 30, LOG_N = 5: " && bash test.sh binary_search_30_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 40, LOG_N = 6: " && bash test.sh binary_search_40_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, LOG_N = 6: " && bash test.sh binary_search_50_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\n"
fi &&

if [[ $# -eq 0 || $1 == 2 ]]; then
  echo -e "--\nTesting Benchmark 2: KMP Search" &&
  echo -e "\nOriginal Method, T2: (N = length of text, M = length of pattern)" &&
  echo -n "N = 20, M = 6: " && bash test.sh kmp_search_20_6 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, M = 12: " && bash test.sh kmp_search_20_12 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, M = 6: " && bash test.sh kmp_search_50_6 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, M = 12: " && bash test.sh kmp_search_50_12 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\nSpecification Method, T1: (N = length of text, M = length of pattern)" &&
  echo -n "N = 20, M = 6: " && bash test.sh kmp_search_20_6_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, M = 12: " && bash test.sh kmp_search_20_12_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, M = 6: " && bash test.sh kmp_search_50_6_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, M = 12: " && bash test.sh kmp_search_50_12_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\n"
fi &&

if [[ $# -eq 0 || $1 == 3 ]]; then
  echo -e "--\nTesting Benchmark 3: Next Permutation" &&
  echo -e "\nOriginal Method, T2: (N = length of array)" &&
  echo -n "N = 10: " && bash test.sh next_permutation_10 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20: " && bash test.sh next_permutation_20 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 30: " && bash test.sh next_permutation_30 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 40: " && bash test.sh next_permutation_40 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50: " && bash test.sh next_permutation_50 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\nSpecification Method, T1: (N = length of array)" &&
  echo -n "N = 10: " && bash test.sh next_permutation_10_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20: " && bash test.sh next_permutation_20_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 30: " && bash test.sh next_permutation_30_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 40: " && bash test.sh next_permutation_40_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50: " && bash test.sh next_permutation_50_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\n"
fi &&

if [[ $# -eq 0 || $1 == 4 ]]; then
  echo -e "--\nTesting Benchmark 4: Recurrence Relations Sequence" &&
  echo -e "\nOriginal Method, T2: (N = number of elements in the sequence, M = number of relations)" &&
  echo -n "N = 20, M = 3: " && bash test.sh rr_sequence_find_20_3 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, M = 6: " && bash test.sh rr_sequence_find_20_6 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, M = 3: " && bash test.sh rr_sequence_find_50_3 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, M = 6: " && bash test.sh rr_sequence_find_50_6 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\nSpecification Method, T1: (N = number of elements in the sequence, M = number of relations)" &&
  echo -n "N = 20, M = 3: " && bash test.sh rr_sequence_find_20_3_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20, M = 6: " && bash test.sh rr_sequence_find_20_6_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, M = 3: " && bash test.sh rr_sequence_find_50_3_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50, M = 6: " && bash test.sh rr_sequence_find_50_6_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\n"
fi &&

if [[ $# -eq 0 || $1 == 5 ]]; then
  echo -e "--\nTesting Benchmark 5: 2D Convex Hull" &&
  echo -e "\nOriginal Method, T2: (N = number of nodes)" &&
  echo -n "N = 10: " && bash test.sh td_convex_hull_10 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20: " && bash test.sh td_convex_hull_20 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 30: " && bash test.sh td_convex_hull_30 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 40: " && bash test.sh td_convex_hull_40 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50: " && bash test.sh td_convex_hull_50 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\nSpecification Method, T1: (N = number of nodes)" &&
  echo -n "N = 10: " && bash test.sh td_convex_hull_10_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 20: " && bash test.sh td_convex_hull_20_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 30: " && bash test.sh td_convex_hull_30_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 40: " && bash test.sh td_convex_hull_40_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "N = 50: " && bash test.sh td_convex_hull_50_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\n"
fi &&

if [[ $# -eq 0 || $1 == 6 ]]; then
  echo -e "--\nTesting Benchmark 6: Maximal Strong Components" &&
  echo -e "\nOriginal Method, T2: (V = number of vertices, E = number of edges)" &&
  echo -n "V = 5, E = 10: " && bash test.sh msc_5_10 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "V = 5, E = 25: " && bash test.sh msc_5_25 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "V = 10, E = 20: " && bash test.sh msc_10_20 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "V = 10, E = 50: " && bash test.sh msc_10_50 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "V = 10, E = 100: " && bash test.sh msc_10_100 | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\nSpecification Method, T1: (V = number of vertices, E = number of edges)" &&
  echo -n "V = 5, E = 10: " && bash test.sh msc_5_10_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "V = 5, E = 25: " && bash test.sh msc_5_25_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "V = 10, E = 20: " && bash test.sh msc_10_20_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "V = 10, E = 50: " && bash test.sh msc_10_50_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -n "V = 10, E = 100: " && bash test.sh msc_10_100_sp | grep --color=never "NUMBER" | sed 's/\:/\ =/g' &&
  echo -e "\n"
fi
