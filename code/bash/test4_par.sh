#!/bin/bash
SIZES=(100000)
THREADS=(2 4 8 16 32 64 128)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./../main -t 4 $SZ
    done
done | tee ../output/output_t4_par.csv
