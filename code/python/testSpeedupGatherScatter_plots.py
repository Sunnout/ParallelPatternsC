"""
@authors: Ema Vieira - 50095, André Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data = pd.read_csv("../output/output_t1_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']

seq_data = pd.read_csv("../output/output_t1_seq.csv") 
seq_data.columns = ['threads', 'size', 'test', 'time']


gather_par = par_data[par_data['test'].str.contains("testGatherMediumFilter")]
scatter_par = par_data[par_data['test'].str.contains("testScatterMediumFilter")]

gather_seq = seq_data[seq_data['test'].str.contains("testGatherMediumFilter")]
scatter_seq = seq_data[seq_data['test'].str.contains("testScatterMediumFilter")]

sG = [gather_seq['time'].values[0],gather_seq['time'].values[0],gather_seq['time'].values[0],gather_seq['time'].values[0],gather_seq['time'].values[0],gather_seq['time'].values[0],gather_seq['time'].values[0]]
sS = [scatter_seq['time'].values[0],scatter_seq['time'].values[0],scatter_seq['time'].values[0],scatter_seq['time'].values[0],scatter_seq['time'].values[0],scatter_seq['time'].values[0],scatter_seq['time'].values[0]]


#Plot
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
plt.plot(gather_par['threads'], sG/gather_par['time'], color='green', linewidth=2, marker='D', linestyle='-', label="Gather")
plt.plot(scatter_par['threads'], sS/scatter_par['time'], color='firebrick', linewidth=2, marker='D', linestyle='-', label="Scatter")
plt.axis([None, None, None, 2.75])
plt.xlabel("Number of Threads \n \n Input Size: 200M", **font)
plt.ylabel("Time Speedup", **font)
plt.title("Time Speedup of Gather and Scatter with medium sized filters", **font)
plt.legend()
plt.savefig('../plots/speedupGatherScatter.pdf', format='pdf')
plt.close()
