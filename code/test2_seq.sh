#!/bin/bash
SIZES=(10000000)
NRUNS=(1 2 3)


for SZ in "${SIZES[@]}"; do
    for R in "${NRUNS[@]}"; do
    echo $SZ
    ./main -s -t 2 $SZ
    done
done | tee output_t2_seq.csv
