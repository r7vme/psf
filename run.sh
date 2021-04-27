#!/bin/bash
set -e
# build docker image with dependencies
docker build -t psf .

# build
docker run -v $(pwd):/code psf /bin/bash -c "cd /code; ./run_build.sh"

if [[ "${1}" == "--perf-test" ]]
then
# performance test
docker run -t -i -v $(pwd):/code psf /bin/bash -c "cd /code; ./run_perf_test.sh"
else
# ping example
docker run -t -i -v $(pwd):/code psf /bin/bash -c "cd /code; ./run_example.sh"
fi

