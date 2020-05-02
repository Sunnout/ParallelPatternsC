#!/bin/bash
SIZES=(500 1000 10000 100000 1000000)


for SZ in "${SIZES[@]}"; do
	echo $SZ
    ./main -s -t 5  $SZ
done | tee output_t5_seq.csv
