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




par_data_7 = pd.read_csv("output_t7_par.csv") 
par_data_7.columns = ['threads', 'size', 'test', 'time']
seq_data_7 = pd.read_csv("output_t7_seq.csv") 
seq_data_7.columns = ['threads', 'size', 'test', 'time']



Hmap_par = par_data_7[par_data_7['test'].str.contains("testMapHeavyWorker")]
Hreduce_par = par_data_6[par_data_6['test'].str.contains("testReduceHeavyWorker")]
Hmap_seq = seq_data_7[seq_data_7['test'].str.contains("testMapHeavyWorker")]
Hreduce_seq = seq_data_6[seq_data_6['test'].str.contains("testReduceHeavyWorker")]


Lmap_par = par_data_0[par_data_0['test'].str.contains("testMapLightWorker")]
Lreduce_par = par_data_0[par_data_0['test'].str.contains("testReduceLightWorker")]
Lmap_seq = seq_data_0[seq_data_0['test'].str.contains("testMapLightWorker")]
Lreduce_seq = seq_data_0[seq_data_0['test'].str.contains("testReduceLightWorker")]



SLmap = (Lmap_seq/Lmap_par).values
SHmap = (Hmap_seq/Hmap_par).values
SLreduce = (Lreduce_seq/Lreduce_par).values
SHreduce = (Hreduce_seq/Hreduce_par).values



print(Hmap_seq)
print(Hmap_par)
print(SHmap)

speedups = [SLmap, SHmap, SLreduce,SHreduce]


plt.bar(x=["Light Map", "Heavy Map", "Light Reduce", "Heavy reduce"],height=speedups)
