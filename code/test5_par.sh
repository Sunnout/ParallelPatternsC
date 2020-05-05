#!/bin/bash
SIZES=(250 500 1000 2000 5000 10000 15000)
THREADS=(4)


for SZ in "${SIZES[@]}"; do
    for TH in "${THREADS[@]}"; do
        echo "Threads, Array Size, Test, Time"
        OMP_NUM_THREADS=$TH ./main -t 5 $SZ
    done
done | tee output_t5_par.csv
