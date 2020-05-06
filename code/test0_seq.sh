#!/bin/bash
SIZES=(200000000)  
THREADS=(1)

for TH in "${THREADS[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for SZ in "${SIZES[@]}"; do
        OMP_NUM_THREADS=$TH ./main -s -t 0 $SZ
    done
done | tee output_t0_seq.csv
