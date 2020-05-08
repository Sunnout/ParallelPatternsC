#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data = pd.read_csv("../output/output_t8_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']

seq_data = pd.read_csv("../output/output_t8_seq.csv") 
seq_data.columns = ['threads', 'size', 'test', 'time']


HfarmS_seq = seq_data[seq_data['test'].str.contains("testFarmHeavyWorkerSmallNumberOfWorkers")]
HfarmM_seq = seq_data[seq_data['test'].str.contains("testFarmHeavyWorkerMediumNumberOfWorkers")]
HfarmL_seq = seq_data[seq_data['test'].str.contains("testFarmHeavyWorkerLargeNumberOfWorkers")]



testFarmHeavyWorkerSmallNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmHeavyWorkerSmallNumberOfWorkers")]

testFarmHeavyWorkerMediumNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmHeavyWorkerMediumNumberOfWorkers")]

testFarmHeavyWorkerLargeNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmHeavyWorkerLargeNumberOfWorkers")]


sS = [HfarmS_seq['time'].values[0],HfarmS_seq['time'].values[0],HfarmS_seq['time'].values[0],HfarmS_seq['time'].values[0],HfarmS_seq['time'].values[0],HfarmS_seq['time'].values[0],HfarmS_seq['time'].values[0]]
sM = [HfarmM_seq['time'].values[0],HfarmM_seq['time'].values[0],HfarmM_seq['time'].values[0],HfarmM_seq['time'].values[0],HfarmM_seq['time'].values[0],HfarmM_seq['time'].values[0],HfarmM_seq['time'].values[0]]
sL = [HfarmL_seq['time'].values[0],HfarmL_seq['time'].values[0],HfarmL_seq['time'].values[0],HfarmL_seq['time'].values[0],HfarmL_seq['time'].values[0],HfarmL_seq['time'].values[0],HfarmL_seq['time'].values[0]]

plt.figure(1, figsize=(12,8))
plt.plot(testFarmHeavyWorkerSmallNumberOfWorkers_par['threads'], sS/testFarmHeavyWorkerSmallNumberOfWorkers_par['time'], '-ro', label="testFarmHeavyWorkerSmallNumberOfWorkers")
plt.plot(testFarmHeavyWorkerMediumNumberOfWorkers_par['threads'], sM/testFarmHeavyWorkerMediumNumberOfWorkers_par['time'], '-go', label="testFarmHeavyWorkerMediumNumberOfWorkers")
plt.plot(testFarmHeavyWorkerLargeNumberOfWorkers_par['threads'], sL/testFarmHeavyWorkerLargeNumberOfWorkers_par['time'], '-bo', label="testFarmHeavyWorkerLargeNumberOfWorkers")


plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Million")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.title("Runtime for different number of Threads")
plt.show()
#plt.savefig('../plots/runtimefarm.png')
plt.close()



