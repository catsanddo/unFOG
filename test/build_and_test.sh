#!/bin/sh

if gcc test/main.c -o tests; then
    ./tests
    rm tests
else
    echo Error in test code!
fi
