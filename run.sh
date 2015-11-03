#!/bin/bash
clear
#echo "Compiling the C++ codes."
cd src
g++ -O3 -o ft1 tweets_cleaned.cpp
g++ -O3 -o ft2 average_degree.cpp

#echo "The C++ codes have compiled successfully."
#echo "Running the first part..."

ft1

#echo "First Part done. Starting second part..."
ft2
#echo "Done"
cd ..


