#!/bin/bash

echo "Starting tests"

numbers="10"
alphas="0.75"

for number in $numbers
do
    for alpha in $alphas
    do
        echo "Trying with context size $number with alpha $alpha"
        ./accuracy.sh $alpha $number
    done
    
done

python3 plot.py
