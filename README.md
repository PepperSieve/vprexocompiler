# Vpr-Exo Compiler #
End-to-end automation to test `vpex` benchmarks in Viper and Pequin

## Testing ##
A dockerfile is provided to build a base docker image to use with projects. Just run:

```bash
./build_docker.sh
```

Please allow the test case to finish to complete the setup.

## Running ##
After compiling the dockerfile, one can run the tests using command:

```bash
docker run -it vprexocompiler bash -c './exec.sh <benchmark>'
```
Currently supported benchmarks are `find_min` and `binary_search`.