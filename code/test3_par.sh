#!/bin/bash
SIZES=(1024)
THREADS=(2 4 8)


for SZ in "${SIZES[@]}"; do
    for TH in "${THREADS[@]}"; do
	echo $SZ $TH
        OMP_NUM_THREADS=$TH ./main -t 3 $SZ
    done
done | tee output_t3_par.csv
