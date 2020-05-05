#!/bin/bash
SIZES=(524288 524289 750000 1048576 1048577 1500000 2097152 2097153 3000000 4194304)
THREADS=(4)

for TH in "${THREADS[@]}"; do
    echo "Threads, Array Size, Test, Time"
    for SZ in "${SIZES[@]}"; do
        OMP_NUM_THREADS=$TH ./main -t 3 $SZ
    done
done | tee output_t3_par.csv
