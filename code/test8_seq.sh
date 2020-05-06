#!/bin/bash
SIZES=(1000000)
THREADS=(1)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./main -s -t 3 $SZ
    done
done | tee output_t8_seq.csv
