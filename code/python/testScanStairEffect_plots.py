#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import pandas as pd 
import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data = pd.read_csv("../output/output_t3_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']


#lightScan_par = par_data[par_data['test'].str.contains("testScanLightWorker")]

heavyScan_par = par_data[par_data['test'].str.contains("testScanHeavyWorker")]


plt.figure(1, figsize=(12,8))
#plt.plot(threads, lightScan_par['time'], '-ro', label="Light Scan")
plt.plot(heavyScan_par['size'], heavyScan_par['time'], '-o', color="firebrick", label="Scan with Heavy Worker")
plt.ylim(bottom=0)
plt.xlabel("Input Size")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.title("Stair Effect in Scan Pattern")
plt.savefig('../plots/stairscan.png')
plt.close()
