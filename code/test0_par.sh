#!/bin/bash
SIZES=(100000000)
THREADS=(16)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./main -t 0 $SZ
    done
done | tee output_t0_par.csv
