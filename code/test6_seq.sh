#!/bin/bash
SIZES=(100000)

for SZ in "${SIZES[@]}"; do
	echo $SZ 
    ./main -s -t 6 $SZ
done | tee output_t6_seq.csv
