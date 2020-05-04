#!/bin/bash
SIZES=(100000)


for SZ in "${SIZES[@]}"; do
    OMP_NUM_THREADS=1 ./main -s -t 1 $SZ  
done | tee output_t1_seq.csv
