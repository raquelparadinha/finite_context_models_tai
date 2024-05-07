#!/bin/bash

cd test/gpt

file_list_gpt=()

accuracy_gpt=0.0

for file in *; do
    if [ -f "$file" ]; then
        file_list_gpt+=("$file")
    fi
done

cd ../..

# Print the list of file names
echo "List of files in test/gpt:"
for file in "${file_list_gpt[@]}"; do
    output=$(make run ARGS="dataset/original dataset/gpt test/gpt/$file 0.5 200")
    if [[ $output == *"Rewritten by ChatGPT"* ]]; then
        echo "Got it right"
        accuracy_gpt=$(awk "BEGIN { print $accuracy_gpt + 1.0 }")
    
    else
        echo "Got it wrong"
    fi
    echo $accuracy_gpt
done

accuracy_gpt=$(awk "BEGIN { print $accuracy_gpt / 6 }")

echo "Final Accuracy for Rewritten ChatGPT: $accuracy_gpt" 

cd test/original

file_list_original=()


accuracy_original=0.0

for file in *; do
    if [ -f "$file" ]; then
        file_list_original+=("$file")
    fi
done

cd ../..

# Print the list of file names
echo "List of files in test/gpt:"
for file in "${file_list_original[@]}"; do
    output=$(make run ARGS="dataset/original dataset/gpt test/original/$file 0.5 200")
    if [[ $output == *"Not rewritten by ChatGPT"* ]]; then
        echo "Got it right"
        accuracy_original=$(awk "BEGIN { print $accuracy_original + 1.0 }")
    
    else
        echo "Got it wrong"
    fi
    echo $accuracy_original
done

accuracy_original=$(awk "BEGIN { print $accuracy_original / 5 }")

echo "Final Accuracy for Not Rewritten ChatGPT: $accuracy_original" 