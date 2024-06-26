#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <alpha> <context_size>"
    exit 1
fi

alpha=$1
context_size=$2

cd test/gpt

file_list_gpt=()

gpt_size=0

accuracy_gpt=0.0

for file in *; do
    if [ -f "$file" ]; then
        file_list_gpt+=("$file")
        echo "$file"
        ((gpt_size += 1))

    fi
done
echo "${#file_list_gpt[@]}"



cd ../..

# Print the list of file names
echo "List of files in test/gpt:"
for file in "${file_list_gpt[@]}"; do
    echo "Running test for $file"
    output=$(make run ARGS="dataset/original dataset/gpt test/gpt/$file $alpha $context_size")
    if [[ $output == *"ChatGPT Text"* ]]; then
        echo "Got it right"
        accuracy_gpt=$(awk "BEGIN { print $accuracy_gpt + 1.0 }")
    
    else
        echo "Got it wrong"
    fi
    echo $accuracy_gpt
done
accuracy_gpt=$(awk "BEGIN { print $accuracy_gpt / $gpt_size }")
accuracy_gpt=$(awk "BEGIN { print $accuracy_gpt * 100 }")

echo "Final Accuracy for Rewritten ChatGPT: $accuracy_gpt %" 

cd test/original

file_list_original=()

accuracy_original=0.0

original_size=0

for file in *; do
    if [ -f "$file" ]; then
        file_list_original+=("$file")
        ((original_size += 1))
    fi
done
echo "${#file_list_original[@]}"

cd ../..

# Print the list of file names
echo "List of files in test/original:"
for file in "${file_list_original[@]}"; do
    echo "Running test for $file"
    output=$(make run ARGS="dataset/original dataset/gpt test/original/$file $alpha $context_size")
    if [[ $output == *"Human Text"* ]]; then
        echo "Got it right"
        accuracy_original=$(awk "BEGIN { print $accuracy_original + 1.0 }")
    else
        echo "Got it wrong"
    fi
    echo $accuracy_original
done

accuracy_original=$(awk "BEGIN { print $accuracy_original / $original_size }")
accuracy_original=$(awk "BEGIN { print $accuracy_original * 100 }")


echo "Final Accuracy for Not Rewritten ChatGPT: $accuracy_original %" 


echo "$alpha,$context_size,$accuracy_gpt,$accuracy_original" >> "results.txt"