#!/bin/bash
SIZES=(100000)

for SZ in "${SIZES[@]}"; do
    OMP_NUM_THREADS=1 ./main -s -t 4 $SZ
done | tee output_t4_seq.csv
