#!/bin/bash
g++ -std=c++11 query.cpp -o query
./query $1/Index.txt < $2 > ./output.txt
