# Vpr-Exo Compiler #
End-to-end automation to test `vpex` benchmarks in Viper and Pequin

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

## Generating Proofs ##
To automatically produce a refinement proof and verify its correctness, enter command:
```bash
cd $VPREXOCOMPILER && ./unifier.sh <benchmark>
```
Currently supported benchmarks are `find_min` and `binary_search`.
