#!/bin/bash
SIZES=(250 500 1000 2000 5000 10000 15000)


for SZ in "${SIZES[@]}"; do
	echo $SZ
    ./main -s -t 5  $SZ
done | tee output_t5_seq.csv
