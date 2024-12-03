#!/bin/bash
# Ask the user for their name
echo Compiling
g++ --std=c++20 $1 "fileRead.cpp" -o $2