#!/bin/bash
SIZES=(100000)

for SZ in "${SIZES[@]}"; do
	echo $SZ
    ./main -s -t 4 $SZ
done | tee output_t4_seq.csv
