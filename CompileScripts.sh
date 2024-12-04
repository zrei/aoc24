#!/bin/bash
echo Compiling
g++ --std=c++20 $1 "fileRead.cpp" -o $2