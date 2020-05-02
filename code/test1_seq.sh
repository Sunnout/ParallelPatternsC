#!/bin/bash
SIZES=(10000)
NRUNS=(1 2 3)


for SZ in "${SIZES[@]}"; do
    for R in "${NRUNS[@]}"; do
    echo $SZ
    ./main -s -t 1 $SZ
    done
done | tee output_t1_seq.csv
