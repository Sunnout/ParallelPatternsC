#!/bin/bash
SIZES=(10000000)



for SZ in "${SIZES[@]}"; do
    echo $SZ
    ./main -s -t 2 $SZ
   
done | tee output_t2_seq.csv
