#!/bin/bash
SIZES=(10000000)
NRUNS=(1 2 3)
THREADS=(2 4 8)


for SZ in "${SIZES[@]}"; do
    for TH in "${THREADS[@]}"; do
        for R in "${NRUNS[@]}"; do
	    echo $SZ $TH
        OMP_NUM_THREADS=$TH ./main -t 2 $SZ
        done
    done
done | tee output_t2_par.csv
