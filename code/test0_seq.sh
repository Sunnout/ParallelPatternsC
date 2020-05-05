#!/bin/bash
SIZES=(200000 500000 1000000 10000000 100000000 200000000)  
THREADS=(1)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./main -s -t 0 $SZ
    done
done | tee output_t0_seq.csv
