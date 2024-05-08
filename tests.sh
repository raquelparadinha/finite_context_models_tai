#!/bin/bash

echo "Starting tests"

numbers="10 50 100 150 200 250 300"
alphas="0.5 0.75 1.00"

for number in $numbers
do
    for alpha in $alphas
    do
        echo "Trying with context size $number with alpha $alpha"
        ./accuracy.sh $alpha $number
    done
    
done

python3 ploy.py
