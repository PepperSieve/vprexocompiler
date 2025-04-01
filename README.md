# Vpr-Exo Compiler：End-to-end testing system for widgets in probabilistic proofs #
Vpr-Exo Compiler, a component of [Distiller](https://eprint.iacr.org/2022/1557), is a tool for writing and verifying refinement proofs that link specifications and implementations in probabilistic proof systems. This repository provides `vprexocompiler`, a tool which interfaces with [pequin](https://github.com/pepper-project/pequin) and [Viper](https://www.pm.inf.ethz.ch/research/viper.html). It reads in customized `vpex` files that represent a computation and automatically constructs and verifies refinement proofs as described in our `vpex` [documentation](https://github.com/PepperSieve/vprexocompiler/blob/main/compiler/documentation.md). It then runs the probabilistic proof protocol on all versions of the computation (TS, TI, and TE), verifying correctness and benchmarking the respective number of constraints generated. The details of this tool are described in our [paper](https://eprint.iacr.org/2022/1557) published at [IEEE S&P (Oakland) 2023](https://www.ieee-security.org/TC/SP2023/index.html).

**PLEASE CLONE USING THE FLAG `--recurse-submodule`!!**

## Running  ##
A dockerfile is provided to build a base docker image to use with projects. Just run:

```bash
./build_docker.sh
```
The building process takes approximately 30 minutes.
Please allow the test case to finish to complete the setup.

After compiling the dockerfile, one needs to first run docker:

```bash
docker run -it vprexocompiler
```

## Generating Constraints and Graphs ##
To generate the constraints for each benchmark and the graph, enter command in the running docker image:
```bash
cd $VPREXOCOMPILER && ./grapher.sh
```
Generated graphs can be found in `$VPREXOCOMPILER/graphs/`.

*Please note that the graph generation process typically takes longer than 12 hours.*

## Generating Proofs ##
To automatically produce a refinement proof and verify its correctness, enter command in the running docker image:
```bash
cd $VPREXOCOMPILER && ./unifier.sh <benchmark>
```
Benchmark Breakdown:
- Y - File can be generated automatically
- A - (For viper) file can be generated automatically, but does not contain annotations
- N - File cannot be generated automatically
- \ - Not applicable (e.g. Te file and TeTs Proof does not exist when Te = Ts)

Benchmarks        | Ti Code | Te Code | Ts Code | TiTe Proof | TeTs Proof
------------------|:-------:|:-------:|:-------:|:----------:|:----------:
`find_min`        | Y | Y | Y | Y | Y
`merging`         | Y | Y | Y | A | A
`binary_search`   | Y | Y | Y | Y | Y
`kmp_search`      | Y | Y | \ | A | \
`next_permutation`| Y | Y | Y | A | A
`dutch_flag`      | Y | Y | Y | A | A
`rr_sequence`     | Y | Y | Y | A | A
`sum_of_powers`   | Y | Y | Y | A | A
`td_convex_hull`  | Y | Y | Y | A | A
`msc`             | Y | Y | Y | A | A
`mst`             | N | N | N | N | N

# Handwritten Proofs
You can also access handwritten viper proofs in `proofs/`. Please refer to the [Viper documentation](https://www.pm.inf.ethz.ch/research/viper.html) for more details on how to verify viper proofs. You can also find information on what backend to use and expected verification time in `proofs/00_sample_runtime_result.txt`.
