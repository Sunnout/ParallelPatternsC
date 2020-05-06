#!/bin/bash
SIZES=(2000000)
THREADS=(1)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./main -s -t 7 $SZ
    done
done | tee output_t7_seq.csv
