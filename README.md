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
Currently, fully supported benchmarks are `find_min` and `binary_search`.
Partially supported benchmarks that can generate C codes and Viper codes without annotation are `merging`, `rr_sequence_find`, `sum_of_powers`, and `td_convex_hull`.