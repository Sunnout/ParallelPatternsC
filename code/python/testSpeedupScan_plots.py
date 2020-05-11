"""
@authors: Ema Vieira - 50095, André Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data_8 = pd.read_csv("../output/output_t7_par.csv") 
par_data_8.columns = ['threads', 'size', 'test', 'time']
seq_data_8 = pd.read_csv("../output/output_t7_seq.csv") 
seq_data_8.columns = ['threads', 'size', 'test', 'time']

s = [seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0]]

#Plot Runtime
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
plt.plot(par_data_8['threads'], s/par_data_8['time'].values, color='darkorange', linewidth=2, marker='D', linestyle='-', label="Parallel Scan with heavy job")
plt.hlines([1,1,1,1,1,1,1], -10, 130, colors='green', linestyles='solid', label='Sequential Scan with heavy job')
plt.axis([-10, 130, 0, 1.25])
plt.xlabel("Number of Threads \n \n Input Size: 2k", **font)
plt.ylabel("Time Speedup", **font)
plt.title("Time Speedup of Parallel Scan with heavy jobs", **font)
plt.legend()
plt.savefig('../plots/speedupScan.pdf', format='pdf')
plt.close()

