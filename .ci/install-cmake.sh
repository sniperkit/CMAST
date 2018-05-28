#!/usr/bin/env bash

CMAKE_URL="https://cmake.org/files/v3.11/cmake-3.11.2.tar.gz"
mkdir cmake
travis_retry wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C cmake
export PATH=${CMAKE_DIR}/cmake/bin:${PATH}
