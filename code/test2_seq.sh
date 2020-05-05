#!/bin/bash
SIZES=(100000000)


echo "Threads, Array Size, Test, Time"
for SZ in "${SIZES[@]}"; do
    OMP_NUM_THREADS=1 ./main -s -t 2 $SZ
done | tee output_t2_seq.csv
