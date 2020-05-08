#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data_8 = pd.read_csv("../output/output_t7_par.csv") 
par_data_8.columns = ['threads', 'size', 'test', 'time']
seq_data_8 = pd.read_csv("../output/output_t7_seq.csv") 
seq_data_8.columns = ['threads', 'size', 'test', 'time']

s = par_data_8[par_data_8['threads'] == 1]

s = [s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0]]



plt.figure(1, figsize=(12,8))
plt.plot(par_data_8['threads'], s/par_data_8['time'].values, '-ro', label="Scan Heavy Worker")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads")
plt.ylabel("SpeedUp")
plt.legend()
plt.title("Scalability for different number of Threads with Input = 1M")
plt.show()
#plt.savefig('../plots/scalescan.png')
plt.close()



