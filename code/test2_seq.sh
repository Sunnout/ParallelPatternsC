#!/bin/bash
SIZES=(100000000)



for SZ in "${SIZES[@]}"; do
    echo "Threads, Array Size, Test, Time"
    OMP_NUM_THREADS=1 ./main -s -t 2 $SZ
done | tee output_t2_seq.csv
