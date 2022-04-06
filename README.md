# Pequin-Automata #
An automated process to test the performance between code that incorporate specifications and original source code of an algorithm.

## Testing ##
A dockerfile is provided to build a base docker image to use with projects. Just run:

```bash
./build_docker.sh
```

## Running ##
After compiling the dockerfile, one can run the tests using command:

```bash
docker run -it pequin bash -c 'cd $PEQUIN/pepper && python2 exec.py <benchmark> <timeout>'
```
Where `<benchmark>` can be:
* `--all`: Test all benchmarks
* `0`: Merging
* `1`: Binary Search
* `2`: KMP Search
* `3`: Next Permutation
* `4`: Dutch Flag
* `5`: Recurrence Relations Sequence
* `6`: Sum of Powers
* `7`: 2D Convex Hull
* `8`: MSC

And `<timeout>` is a positive number for number of seconds, or defaulted to 600 seconds.

## Example ##
```bash
docker run -it pequin bash -c 'cd $PEQUIN/pepper && python2 exec.py 1 30'
```
Runs the Binary Search benchmark, and test terminates if last longer than 30 seconds.

## Debugging ##
Warnings and errors are stored in `$PEQUIN/pepper/log`.