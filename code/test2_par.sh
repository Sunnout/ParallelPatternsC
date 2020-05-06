#!/bin/bash
SIZES=(2000)
THREADS=(1 2 4 8 16 32 64 128)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"  
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./main -t 2 $SZ
    done
done | tee output_t2_par.csv
