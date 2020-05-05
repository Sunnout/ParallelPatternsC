#!/bin/bash
SIZES=(100000)
THREADS=(4)


echo "Threads, Array Size, Test, Time"
for SZ in "${SIZES[@]}"; do
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./main -t 6 $SZ
    done
done | tee output_t6_par.csv
