#!/bin/bash
SIZES=(250 500 1000 2000 5000 10000 15000)


for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    OMP_NUM_THREADS=1 ./main -s -t 5  $SZ
done | tee output_t5_seq.csv
