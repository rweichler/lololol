#!/usr/bin/env bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 YourExecutable SeansExecutable"
    exit 1
fi

dir="LMAOtmplololol"
mkdir -p $dir
for M in $(seq 2 20); do
    for L in $(seq 2 20); do
        for text in $(ls *.txt); do
            x=$(./$1 $text $M $L > $dir/reed 2>/dev/null)
            ./$2 $text $M $L > $dir/sean
            if [[ -n $(diff $dir/reed $dir/sean) ]]; then
                echo "./$1 $text $M $L"
            fi
        done
    done
done
rm -rf $dir
