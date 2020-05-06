#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data = pd.read_csv("../output/output_t1_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']



gather_par = par_data[par_data['test'].str.contains("testGatherMediumFilter")]

scatter_par = par_data[par_data['test'].str.contains("testScatterMediumFilter")]

map_par = par_data[par_data['test'].str.contains("testMapLightWorker")]


plt.figure(1, figsize=(12,8))
plt.plot(gather_par['threads'], gather_par['time'], '-ro', label="Gather")
plt.plot(scatter_par['threads'], scatter_par['time'], '-bo', label="Scatter")
plt.plot(map_par['threads'], map_par['time'], '-go', label="Light Map")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Million")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.title("Runtime for different number of Threads")
plt.show()
#plt.savefig('../plots/runtimemgs.png')
plt.close()



