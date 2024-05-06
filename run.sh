#!/bin/bash
# This is run_app.sh

# Ensure Python is installed
if ! command -v python3 &> /dev/null
then
    echo "Python could not be found, please install Python 3."
    exit
fi

# Run the Python script
python3 gui/gui.py

# Check if input file exists
if [ ! -f inputs.txt ]; then
    echo "No inputs file found."
    exit 1
fi

# Read contents of the file
inputs=$(<inputs.txt)

make clean
make all

# Run the C++ program with the inputs
./bin/markov_model_classifier $inputs