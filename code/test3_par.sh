#!/bin/bash
SIZES=(2048 2049 3000 4096 4097 6000 8192 8193 12000 16384)
THREADS=(4)

for TH in "${THREADS[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for SZ in "${SIZES[@]}"; do
        OMP_NUM_THREADS=$TH ./main -t 3 $SZ
    done
done | tee output_t3_par.csv
