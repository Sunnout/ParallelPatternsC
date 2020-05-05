#!/bin/bash
SIZES=(100000000)
THREADS=(2 4 8)


for SZ in "${SIZES[@]}"; do
    for TH in "${THREADS[@]}"; do
        echo "Threads, Array Size, Test, Time"
        OMP_NUM_THREADS=$TH ./main -t 2 $SZ
    done
done | tee output_t2_par.csv
