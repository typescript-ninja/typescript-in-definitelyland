#!/bin/bash

set -eux

rm -rf node_modules
yarn install

git submodule init && git submodule update
