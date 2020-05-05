#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data = pd.read_csv("output_t3_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']


lightScan_par = par_data[par_data['test'].str.contains("testScanLightWorker")]

heavyScan_par = par_data[par_data['test'].str.contains("testScanHeavyWorker")]


threads = [2,4,8]

plt.figure(1, figsize=(12,8))
plt.plot(threads, lightScan_par['time'], '-ro', label="Light Scan")
plt.plot(threads, heavyScan_par['time'], '-bo', label="Heavy Scan")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 1024")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.show()
plt.title("Runtime for different number of Threads")
plt.savefig('plots/test3_runtime.png')
plt.close()
