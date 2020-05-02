#!/bin/bash
SIZES=(100000)


for SZ in "${SIZES[@]}"; do
    echo $SZ
    ./main -s -t 1 $SZ
    
done | tee output_t1_seq.csv
