#!/bin/bash
SIZES=(20000)
THREADS=(32)

for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./../main -t 5 $SZ
    done
done | tee ../output/output_t5_par.csv
