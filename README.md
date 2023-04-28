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
- N - File cannot be generated automatically
- \ - Not applicable (e.g. Te file and TeTs Proof does not exist when Te = Ts)

Benchmarks        | Ti Code | Te Code | Ts Code | TiTe Proof | TeTs Proof
------------------|---------|---------|---------|------------|------------
Find Min          | Y | Y | Y | Y | Y
Merging           | Y | Y | Y | A | A
Binary Search     | Y | Y | Y | Y | Y
KMP Search        | Y | Y | \ | A | \
Next Permutation  | Y | Y | Y | A | A
Dutch Flag        | Y | Y | Y | A | A
RR Sequence       | Y | Y | Y | A | A
Sum of Powers     | Y | Y | Y | A | A
2D Convex HUll    | Y | Y | Y | A | A
MSC               | Y | Y | Y | A | A
MST               | N | N | N | N | N