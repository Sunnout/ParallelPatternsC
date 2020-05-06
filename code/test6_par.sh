#!/bin/bash
SIZES=(2000000)
THREADS=(2 4 8 16 32 64 128)


for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./main -t 6 $SZ
    done
done | tee output_t6_par.csv
