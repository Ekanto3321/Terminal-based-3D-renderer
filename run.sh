#!/bin/bash
gcc -o spin spin.c -lm

if [ $? -eq 0 ]; then
    ./spin
else
    echo "didn't compile"
fi
