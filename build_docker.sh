#!/bin/bash

set -e
 
docker build -t pequin -f docker/Dockerfile .

# Test created image
docker run -it pequin bash -c 'cd $PEQUIN/pepper && python2 exec.py --all 10'
