#!/bin/bash

set -e
 
docker build -t vprexocompiler -f docker/Dockerfile .

# Test created image
docker run -it vprexocompiler bash -c 'bash exec.sh find_min'