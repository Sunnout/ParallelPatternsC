#!/bin/bash
SIZES=(100)


echo "Threads, Array Size, Test, Time"
for SZ in "${SIZES[@]}"; do
    OMP_NUM_THREADS=1 ./main -s -t 6 $SZ
done | tee output_t6_seq.csv
