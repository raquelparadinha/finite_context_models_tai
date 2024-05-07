#!/bin/bash

# Input CSV file
input_file="article-class.csv"

# Create directories for the two classes
mkdir -p class_0_articles class_1_articles

# Counter for file naming
counter=1

# Read the CSV file line by line
tail -n +2 "$input_file" | while IFS=, read -r line; do
    # Extract the last field
    class=$(echo "$line" | awk -F ',' '{print $NF}')
    
    # Extract the article content
    article=$(echo "$line" | awk -F ',' '{$NF=""; print}' | sed 's/"//g')

    # Determine the destination folder based on the last field
    if [ "$class" -eq 0 ]; then
        destination_folder="class_0_articles"
    elif [ "$class" -eq 1 ]; then
        destination_folder="class_1_articles"
    else
        echo "Invalid class value: $class"
        continue
    fi

    # Save the article to the appropriate folder with a sequential filename
    echo "$article" > "$destination_folder/article$counter.txt"
    
    # Increment the counter
    ((counter++))
done