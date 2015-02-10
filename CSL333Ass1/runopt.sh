#!/bin/bash
# Call this script with at least 3 parameters, for example
# sh scriptname 1 2 3

g++ -std=c++11 main.cpp
./a.out $1 $2 
exit 0