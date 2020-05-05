#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May  5 14:42:32 2020

@author: emarodriguesvieira
"""
"""
Script to make plots
"""

import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data = pd.read_csv("output_t2_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']
seq_data = pd.read_csv("output_t2_seq.csv") 
seq_data.columns = ['threads', 'size', 'test', 'time']


gather_par = par_data[par_data['test'].str.contains("testGatherMediumFilter")]

scatter_par = par_data[par_data['test'].str.contains("testScatterMediumFilter")]

gather_seq = seq_data[seq_data['test'].str.contains("testGatherMediumFilter")]

scatter_seq = seq_data[seq_data['test'].str.contains("testScatterMediumFilter")]


threads = [2,4,8]

plt.figure(1, figsize=(12,8))
plt.plot(threads, gather_par['time'], '-ro', label="Gather")
plt.plot(threads, scatter_par['time'], '-bo', label="Scatter")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Million")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.show()
plt.title("Runtime for different number of Threads")
#plt.savefig('plots/test2_runtime.png')
plt.close()

g = [gather_seq['time'].values[0], gather_seq['time'].values[0],gather_seq['time'].values[0]]

s = [scatter_seq['time'].values[0],scatter_seq['time'].values[0],scatter_seq['time'].values[0]]

plt.figure(1, figsize=(12,8))
plt.plot(threads, g/gather_par['time'].values, '-ro', label="Gather")
plt.plot(threads, s/scatter_par['time'].values, '-bo', label="Scatter")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Million")
plt.ylabel("Time SpeedUp")
plt.legend()
plt.show()
plt.title("Time Speedup for different number of Threads")
#plt.savefig('plots/test2_speedup.png')
plt.close()



