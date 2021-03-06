#!/bin/bash
SIZES=(5000)
THREADS=(1)


for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for TH in "${THREADS[@]}"; do
        OMP_NUM_THREADS=$TH ./../main -s -t 6 $SZ
    done
done | tee ../output/output_t6_seq.csv