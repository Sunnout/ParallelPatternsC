#!/bin/bash
SIZES=(100000)
THREADS=(4)


for SZ in "${SIZES[@]}"; do
    for TH in "${THREADS[@]}"; do
	echo $SZ $TH
        OMP_NUM_THREADS=$TH ./main -t 6 $SZ
    done
done | tee output_t6_par.csv
