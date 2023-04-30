# Vpr-Exo Compiler #
End-to-end automation to test `vpex` benchmarks in Viper and Pequin

**PLEASE CLONE USING THE FLAG `--recurse-submodule`!!**

## Running  ##
A dockerfile is provided to build a base docker image to use with projects. Just run:

```bash
./build_docker.sh
```
Please allow the test case to finish to complete the setup.
After compiling the dockerfile, one needs to first run docker:

```bash
docker run -it vprexocompiler
```

## Generating Constraints and Graphs ##
To generate the constraints for each benchmark and the graph, enter command:
```bash
cd $VPREXOCOMPILER && ./grapher.sh
```
Generated graphs can be found in `$VPREXOCOMPILER/graphs/`.

## Generating Proofs ##
To automatically produce a refinement proof and verify its correctness, enter command:
```bash
cd $VPREXOCOMPILER && ./unifier.sh <benchmark>
```
Benchmark Breakdown:
- Y - File can be generated automatically
- A - (For viper) file can be generated automatically without annotations
- C - (For C) can generate constraints, but not verification (lacking exo & inp_gen)
- N - File cannot be generated automatically
- \ - Not applicable (e.g. Te file and TeTs Proof does not exist when Te = Ts)

Benchmarks        | Ti Code | Te Code | Ts Code | TiTe Proof | TeTs Proof
------------------|---------|---------|---------|------------|------------
`find_min`        | Y | Y | Y | Y | Y
`merging`         | Y | Y | Y | A | A
`binary_search`   | Y | Y | Y | Y | Y
`kmp_search`      | C | C | \ | A | \
`next_permutation`| Y | Y | C | A | A
`dutch_flag`      | Y | C | Y | A | A
`rr_sequence`     | Y | Y | Y | A | A
`sum_of_powers`   | Y | Y | Y | A | A
`td_convex_hull`  | Y | Y | Y | A | A
`msc`             | C | C | C | A | A
`mst`             | N | N | N | N | N