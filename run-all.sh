#!/bin/bash

if [ "$1" == "" ]; then
    echo "Time limit parameter is empty"
    exit 1
fi

make

output_dir="out"
mkdir $output_dir


for fullfilename in rl-tp-instancias/*
    do
        filename=$(basename "$fullfilename")
        fname="${filename%.*}"
        ./relaxlag $fullfilename $output_dir/$fname.out $1
        echo $filename $fname

    done