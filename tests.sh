#!/bin/bash

echo "Starting tests"

numbers="10 50 100 150 200 250 300"
alphas="0.25 0.5 0.75 1.00"

for number in $numbers
do
    for alpha in $alphas
    do
        echo "Trying with context size $number with alpha $alpha"
        make run ARGS="dataset/original dataset/gpt dataset/test_text.txt $alpha $number"
    done

    
done
