#!/bin/bash
SIZES=(5000000)
THREADS=(1)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"  
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./../main -s -t 10 $SZ
    done
done | tee ../output/output_t10_seq.csv
