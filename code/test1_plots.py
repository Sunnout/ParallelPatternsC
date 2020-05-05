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
par_data = pd.read_csv("output_t1_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']
seq_data = pd.read_csv("output_t1_seq.csv") 
seq_data.columns = ['threads', 'size', 'test', 'time']


map_par = par_data[par_data['test'].str.contains("testMapHeavyWorker")]
reduce_par = par_data[par_data['test'].str.contains("testReduceHeavyWorker")]
pack_par = par_data[par_data['test'].str.contains("testPackMediumFilterAlternated")]
pipeline_par = par_data[par_data['test'].str.contains("testPipelineMediumWorkersSmallNumberOfStations")]
farm_par = par_data[par_data['test'].str.contains("testFarmHeavyWorkerLargeNumberOfWorkers")]

map_seq = seq_data[seq_data['test'].str.contains("testMapHeavyWorker")]
reduce_seq = seq_data[seq_data['test'].str.contains("testReduceHeavyWorker")]
pack_seq = seq_data[seq_data['test'].str.contains("testPackMediumFilterAlternated")]
pipeline_seq = seq_data[seq_data['test'].str.contains("testPipelineMediumWorkersSmallNumberOfStations")]
farm_seq = seq_data[seq_data['test'].str.contains("testFarmHeavyWorkerLargeNumberOfWorkers")]


threads = [2,4,8]

plt.figure(1, figsize=(12,8))
plt.plot(threads, map_par['time'], '-ro', label="Map")
plt.plot(threads, pack_par['time'], '-go', label="Pack")
plt.plot(threads, pipeline_par['time'], '-yo', label="Pipeline")
plt.plot(threads, farm_par['time'], '-mo', label="Farm")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Thousand")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.show()
plt.title("Runtime for different number of Threads")
#plt.savefig('plots/test1_runtime.png')
plt.close()

plt.figure(1, figsize=(12,8))
plt.plot(threads, reduce_par['time'], '-bo', label="Reduce")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Thousand")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.show()
plt.title("Reduce Runtime for different number of Threads")
#plt.savefig('plots/test1_reduce_runtime.png')
plt.close()

m = [map_seq['time'].values[0], map_seq['time'].values[0],map_seq['time'].values[0]]
r = [reduce_seq['time'].values[0],reduce_seq['time'].values[0],reduce_seq['time'].values[0]]
pa = [pack_seq['time'].values[0], pack_seq['time'].values[0],pack_seq['time'].values[0]]
pi = [pipeline_seq['time'].values[0],pipeline_seq['time'].values[0],pipeline_seq['time'].values[0]]
f = [farm_seq['time'].values[0], farm_seq['time'].values[0],farm_seq['time'].values[0]]

plt.figure(1, figsize=(12,8))
plt.plot(threads, r/reduce_par['time'].values, '-bo', label="Reduce")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Thousand")
plt.ylabel("Time SpeedUp")
plt.legend()
plt.show()
plt.title("Reduce Time Speedup for different number of Threads")
#plt.savefig('plots/test1_reduce_speedup.png')
plt.close()

plt.figure(1, figsize=(12,8))
plt.plot(threads, m/map_par['time'].values, '-ro', label="Map")
plt.plot(threads, pa/pack_par['time'].values, '-go', label="Pack")
plt.plot(threads, pi/pipeline_par['time'].values, '-yo', label="Pipeline")
plt.plot(threads, f/farm_par['time'].values, '-mo', label="Farm")
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Thousand")
plt.ylabel("Time SpeedUp")
plt.legend()
plt.show()
plt.title("Time Speedup for different number of Threads")
#plt.savefig('plots/test1_speedup.png')
plt.close()



