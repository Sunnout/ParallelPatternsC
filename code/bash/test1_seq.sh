#!/bin/bash
SIZES=(200000000)
THREADS=(1)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./../main -s -t 1 $SZ
    done
done | tee ../output/output_t1_seq.csv
