#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np


# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data_0 = pd.read_csv("output_t0_par.csv") 
par_data_0.columns = ['threads', 'size', 'test', 'time']
seq_data_0 = pd.read_csv("output_t0_seq.csv") 
seq_data_0.columns = ['threads', 'size', 'test', 'time']


par_data_6 = pd.read_csv("output_t6_par.csv") 
par_data_6.columns = ['threads', 'size', 'test', 'time']
seq_data_6 = pd.read_csv("output_t6_seq.csv") 
seq_data_6.columns = ['threads', 'size', 'test', 'time']




par_data_5 = pd.read_csv("output_t5_par.csv") 
par_data_5.columns = ['threads', 'size', 'test', 'time']
seq_data_7 = pd.read_csv("output_t7_seq.csv") 
seq_data_7.columns = ['threads', 'size', 'test', 'time']



Hmap_par = par_data_5[par_data_5['test'].str.contains("testMapHeavyWorker")]
Hreduce_par = par_data_6[par_data_6['test'].str.contains("testReduceHeavyWorker") & (par_data_6['threads'] == 16)]
Hmap_seq = seq_data_7[seq_data_7['test'].str.contains("testMapHeavyWorker")]
Hreduce_seq = seq_data_6[seq_data_6['test'].str.contains("testReduceHeavyWorker")]


Lmap_par = par_data_0[par_data_0['test'].str.contains("testMapLightWorker") & (par_data_0['size'] == 200000000)]
Lreduce_par = par_data_0[par_data_0['test'].str.contains("testReduceLightWorker") & (par_data_0['size'] == 200000000)]
Lmap_seq = seq_data_0[seq_data_0['test'].str.contains("testMapLightWorker") & (par_data_0['size'] == 200000000)]
Lreduce_seq = seq_data_0[seq_data_0['test'].str.contains("testReduceLightWorker") & (par_data_0['size'] == 200000000)]



SLmap = Lmap_seq['time'].values/Lmap_par['time'].values
SHmap = Hmap_seq['time'].values/Hmap_par['time'].values
SLreduce = Lreduce_seq['time'].values/Lreduce_par['time'].values
SHreduce = Hreduce_seq['time'].values/Hreduce_par['time'].values



print(Hmap_seq)
print(Hmap_par)

speedups = [SLmap[0], SHmap[0], SLreduce[0],SHreduce[0]]


print(speedups)


teamColours = ['navajowhite','orange','navajowhite','orange']

plt.bar(x=["Light Map", "Heavy Map", "Light Reduce", "Heavy reduce"],height=speedups,color=teamColours)
plt.title("Time SpeedUp with Light vs Heavy Workers")
plt.ylabel("Time SpeedUp")
plt.show()
