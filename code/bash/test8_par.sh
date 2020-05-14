#!/bin/bash
SIZES=(20000)
THREADS=(2 4 8 16 32 64 128)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./../main -t 8 $SZ
    done
done | tee ../output/output_t8_par.csv
