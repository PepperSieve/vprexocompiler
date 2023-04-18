# Pequin-Automata #
An automated process to test the performance between code that incorporate specifications and original source code of an algorithm.

This is a fork of the original Pequin compiler: https://github.com/pepper-project/pequin

## Testing ##
A dockerfile is provided to build a base docker image to use with projects. Just run:

```bash
./build_docker.sh
```

## Generating the Graph ##
After compiling the dockerfile, one can generate the graph using command:

```bash docker run -it pequin bash -c 'cd $PEQUIN && bash ./default.sh'```

Note that graphs might look slightly different depending on processor speed and
how many cases can be processed before reaching timeout limit.

## Other Testings ##
One can also test each individual benchmarks by using the command:

```bash
docker run -it pequin bash -c 'cd $PEQUIN/pepper && python2 exec.py <benchmark> <timeout>'
```
Where `<benchmark>` can be:
* `--all`: Test all benchmarks
* `0`: Find Min
* `1`: Merging
* `2`: Binary Search
* `3`: KMP Search
* `4`: Next Permutation
* `5`: Dutch Flag
* `6`: Recurrence Relations Sequence
* `7`: Sum of Powers
* `8`: 2D Convex Hull
* `9`: MSC

And `<timeout>` is a positive number for number of seconds, or defaulted to 600 seconds.

## Example ##
```bash
docker run -it pequin bash -c 'cd $PEQUIN/pepper && python2 exec.py 2 30'
```
Runs the Binary Search benchmark, and test terminates if last longer than 30 seconds.

## Debugging ##
Warnings and errors are stored in `$PEQUIN/pepper/log`.