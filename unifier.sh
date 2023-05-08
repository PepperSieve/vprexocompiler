#! /bin/bash
cd compiler &&
mkdir -p bin &&
g++ -std=c++11 src/compiler.cpp -o bin/compiler &&

echo -e "\n\n+------------------------------+" &&
echo -e "| Generating c and vpr file... |" &&
echo -e "+------------------------------+\n" &&
./bin/compiler -a ./data/$1 &&
cd .. &&
mkdir -p pequin/pepper/src/ &&
mkdir -p pequin/pepper/input_generation/ &&
cp compiler/data/$1_exo.c pequin/pepper/src/ &&
cp compiler/data/$1_ti.c pequin/pepper/apps/ &&
cp compiler/data/$1_te.c pequin/pepper/apps/ &&
cp compiler/data/$1_ts.c pequin/pepper/apps/ &&
cp compiler/data/$1_ti_v_inp_gen.h pequin/pepper/input_generation/ &&
cp compiler/data/$1_te_v_inp_gen.h pequin/pepper/input_generation/ &&
cp compiler/data/$1_ts_v_inp_gen.h pequin/pepper/input_generation/ &&
cd viper/silicon &&

if [ "$1" == "find_min" ] || [ "$1" == "binary_search" ]; then

    echo -e "\n\n+---------------------------+" &&
    echo -e "| Verifying Te->Ts Proof... |" &&
    echo -e "+---------------------------+\n" &&
    sbt "run --z3Exe=../z3/bin/z3 ../../compiler/data/$1_TeTs.vpr" &&

    echo -e "\n\n+---------------------------+" &&
    echo -e "| Verifying Ti->Te Proof... |" &&
    echo -e "+---------------------------+\n" &&
    sbt "run --z3Exe=../z3/bin/z3 ../../compiler/data/$1_TiTe.vpr" &&

    echo -e "\n\n+-----------------------+" &&
    echo -e "| Compiling EXO File... |" &&
    echo -e "+-----------------------+\n" &&
    cd ../../pequin/pepper &&
    gcc src/$1_exo.c -o bin/exo1 &&
    echo -e "EXO file successfully compiled" &&

    echo -e "\n\n+-------------------------+" &&
    echo -e "| Running Ti in Pequin... |" &&
    echo -e "+-------------------------+\n" &&
    bash test.sh $1_ti | grep "CONSTRAINTS" &&
    bin/pepper_verifier_$1_ti gen_input $1_ti.inputs &&
    bin/pepper_prover_$1_ti prove $1_ti.pkey $1_ti.inputs $1_ti.outputs $1_ti.proof &&
    bin/pepper_verifier_$1_ti verify $1_ti.vkey $1_ti.inputs $1_ti.outputs $1_ti.proof &&

    echo -e "\n\n+-------------------------+" &&
    echo -e "| Running Te in Pequin... |" &&
    echo -e "+-------------------------+\n" &&
    bash test.sh $1_te | grep "CONSTRAINTS" &&
    bin/pepper_verifier_$1_te gen_input $1_te.inputs &&
    bin/pepper_prover_$1_te prove $1_te.pkey $1_te.inputs $1_te.outputs $1_te.proof &&
    bin/pepper_verifier_$1_te verify $1_te.vkey $1_te.inputs $1_te.outputs $1_te.proof &&

    echo -e "\n\n+-------------------------+" &&
    echo -e "| Running Ts in Pequin... |" &&
    echo -e "+-------------------------+\n" &&
    bash test.sh $1_ts | grep "CONSTRAINTS" &&
    bin/pepper_verifier_$1_ts gen_input $1_ts.inputs &&
    bin/pepper_prover_$1_ts prove $1_ts.pkey $1_ts.inputs $1_ts.outputs $1_ts.proof &&
    bin/pepper_verifier_$1_ts verify $1_ts.vkey $1_ts.inputs $1_ts.outputs $1_ts.proof

elif [ "$1" == "merging" ] || [ "$1" == "kmp_search" ] || [ "$1" == "sum_of_powers" ] || [ "$1" == "next_permutation" ] || [ "$1" == "dutch_flag" ] || [ "$1" == "rr_sequence_find" ] || [ "$1" == "td_convex_hull" ]  || [ "$1" == "msc" ]; then

    echo -e "\n----\nPlease view generated Viper codes in compiler/data/\n----" &&

    echo -e "\n\n+-----------------------+" &&
    echo -e "| Compiling EXO File... |" &&
    echo -e "+-----------------------+\n" &&
    cd ../../pequin/pepper &&
    gcc src/$1_exo.c -o bin/exo1 &&
    echo -e "EXO file successfully compiled" &&

    # echo -e "\n\n+-------------------------+" &&
    # echo -e "| Running Ti in Pequin... |" &&
    # echo -e "+-------------------------+\n" &&
    # bash test.sh $1_ti | grep "CONSTRAINTS" &&
    # bin/pepper_verifier_$1_ti gen_input $1_ti.inputs &&
    # bin/pepper_prover_$1_ti prove $1_ti.pkey $1_ti.inputs $1_ti.outputs $1_ti.proof &&
    # bin/pepper_verifier_$1_ti verify $1_ti.vkey $1_ti.inputs $1_ti.outputs $1_ti.proof &&

    echo -e "\n\n+-------------------------+" &&
    echo -e "| Running Te in Pequin... |" &&
    echo -e "+-------------------------+\n" &&
    bash test.sh $1_te | grep "CONSTRAINTS" &&
    bin/pepper_verifier_$1_te gen_input $1_te.inputs &&
    bin/pepper_prover_$1_te prove $1_te.pkey $1_te.inputs $1_te.outputs $1_te.proof &&
    bin/pepper_verifier_$1_te verify $1_te.vkey $1_te.inputs $1_te.outputs $1_te.proof &&

    if [ "$1" != "kmp_search" ]; then
        echo -e "\n\n+-------------------------+" &&
        echo -e "| Running Ts in Pequin... |" &&
        echo -e "+-------------------------+\n" &&
        bash test.sh $1_ts | grep "CONSTRAINTS" &&
        bin/pepper_verifier_$1_ts gen_input $1_ts.inputs &&
        bin/pepper_prover_$1_ts prove $1_ts.pkey $1_ts.inputs $1_ts.outputs $1_ts.proof &&
        bin/pepper_verifier_$1_ts verify $1_ts.vkey $1_ts.inputs $1_ts.outputs $1_ts.proof
    fi

else

    echo -e "\n----\nPlease view generated Viper codes in compiler/data/\n----" &&

    cd ../../pequin/pepper &&

    echo -e "\n\n+-------------------------+" &&
    echo -e "| Running Ti in Pequin... |" &&
    echo -e "+-------------------------+\n" &&
    bash test.sh $1_ti | grep "CONSTRAINTS" &&

    echo -e "\n\n+-------------------------+" &&
    echo -e "| Running Te in Pequin... |" &&
    echo -e "+-------------------------+\n" &&
    bash test.sh $1_te | grep "CONSTRAINTS" &&

    if [ "$1" != "kmp_search" ]; then

        echo -e "\n\n+-------------------------+" &&
        echo -e "| Running Ts in Pequin... |" &&
        echo -e "+-------------------------+\n" &&
        bash test.sh $1_ts | grep "CONSTRAINTS"

    fi

fi