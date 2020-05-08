#!/bin/bash
SIZES=(5000)
THREADS=(1)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"  
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./../main -s -t 2 $SZ
    done
done | tee ../output/output_t2_seq.csv
