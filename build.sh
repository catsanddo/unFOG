#!/bin/sh

flags="-g"

mkdir -p bin

cd bin
gcc ../src/unfog.c $flags -o unfog
cd ..
