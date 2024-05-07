#!/bin/bash

# Input file containing data
input_file="GPT-wiki-intro.csv"

# Output file to store the first 10 million entries
output_file="test.csv"

# Use head command to extract the first 10 million lines
awk -F, 'NR > 1 {print $4","$10}' "$input_file" | head -n 15000 > "$output_file"

awk -F, 'NR <= 2 {print}' "$input_file"

