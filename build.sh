#!/bin/sh

flags="-g -I../include"

mkdir -p bin

cd bin
gcc ../src/unfog.c $flags -o unfog
cd ..
