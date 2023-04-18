#! /bin/bash
cd compiler &&
mkdir -p bin &&
g++ -std=c++11 src/compiler.cpp -o bin/compiler &&

echo -e "\n\n+------------------------------+" &&
echo -e "| Generating c file... |" &&
echo -e "+------------------------------+\n" &&
echo -e "--\nCompiling find_min:"
./bin/compiler -p ./data/find_min &&
echo -e "--\nCompiling merging:"
./bin/compiler -p ./data/merging &&
echo -e "--\nCompiling binary_search:"
./bin/compiler -p ./data/binary_search &&
echo -e "--\nCompiling kmp_search:"
./bin/compiler -p ./data/kmp_search &&
echo -e "--\nCompiling next_permutation:"
./bin/compiler -p ./data/next_permutation &&
echo -e "--\nCompiling dutch_flag:"
./bin/compiler -p ./data/dutch_flag &&
echo -e "--\nCompiling rr_sequence_find:"
./bin/compiler -p ./data/rr_sequence_find &&
echo -e "--\nCompiling sum_of_powers:"
./bin/compiler -p ./data/sum_of_powers &&
echo -e "--\nCompiling 2d_convex_hull:"
./bin/compiler -p ./data/td_convex_hull &&
echo -e "--\nCompiling msc:"
./bin/compiler -p ./data/msc &&
cd .. &&
cp compiler/data/*_ti.c pequin/pepper/skeletons/ &&
cp compiler/data/*_te.c pequin/pepper/skeletons/ &&

echo -e "\n\n+-------------------------+" &&
echo -e "| Testing in Pequin... |" &&
echo -e "+-------------------------+\n" &&
cd pequin/pepper &&
python3 exec.py --all &&
python3 latex_generator.py
