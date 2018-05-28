#!/usr/bin/env bash

CMAKE_URL="https://cmake.org/files/v3.8/cmake-3.8.2-Linux-x86_64.tar.gz"
mkdir cmake
travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
export PATH=${CMAKE_DIR}/cmake/bin:${PATH}
