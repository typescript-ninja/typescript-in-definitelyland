#!/bin/bash -eux

docker run -t --rm \
  -v $(pwd):/book vvakame/review \
  /bin/bash -ci "cd /book && ./setup.sh && npm run pdf"
