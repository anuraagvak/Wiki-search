#!/bin/bash
make
./step1 $1 
mv ./data/Index.txt $2/Index.txt
