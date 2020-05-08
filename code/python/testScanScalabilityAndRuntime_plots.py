"""
@authors: Ema Vieira - 50095, André Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data_8 = pd.read_csv("../output/output_t7_par.csv") 
par_data_8.columns = ['threads', 'size', 'test', 'time']
seq_data_8 = pd.read_csv("../output/output_t7_seq.csv") 
seq_data_8.columns = ['threads', 'size', 'test', 'time']

s = par_data_8[par_data_8['threads'] == 1]
s = [s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0]]


#Plot Scalability
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
plt.plot(par_data_8['threads'], s/par_data_8['time'].values, color='darkorange', linewidth=2, marker='D', linestyle='-',label="Scan with heavy job")
plt.axis([None, None, None, 3.3])
plt.xlabel("Number of Threads \n \n Input Size: 2k", **font)
plt.ylabel("Speedup (Parallel with 1 thread/Parallel with x threads)", **font)
plt.title("Scalability of Parallel Scan for increasing number of threads", **font)
plt.legend()
plt.savefig('../plots/scalabilityScan.eps', format='eps')
plt.close()



s = [seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0],seq_data_8['time'].values[0]]

#Plot Runtime
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
plt.plot(par_data_8['threads'], par_data_8['time'].values, color='darkorange', linewidth=2, marker='D', linestyle='-', label="Parallel Scan with heavy job")
plt.hlines(s, -10, 130, colors='mediumvioletred', linestyles='solid', label='Sequential Scan with heavy job')
plt.axis([-10, 130, 3, 20])
plt.xlabel("Number of Threads \n \n Input Size: 2k", **font)
plt.ylabel("Runtime (seconds)", **font)
plt.title("Runtime of Parallel Scan with heavy jobs vs. Sequential Scan with heavy jobs", **font)
plt.legend()
plt.savefig('../plots/RuntimeSeqvsParScan.eps', format='eps')
plt.close()

