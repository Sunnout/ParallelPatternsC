"""
@authors: Ema Vieira - 50095, André Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data = pd.read_csv("../output/output_t1_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']



gather_par = par_data[par_data['test'].str.contains("testGatherMediumFilter")]
scatter_par = par_data[par_data['test'].str.contains("testScatterMediumFilter")]


#Plot
font = {'fontname':'Arial'}
plt.figure(1, figsize=(10,8))
plt.plot(gather_par['threads'], gather_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-', label="Gather")
plt.plot(scatter_par['threads'], scatter_par['time'], color='olivedrab', linewidth=2, marker='D', linestyle='-', label="Scatter")
plt.axis([None, None, None, 10])
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n \n Input Size: 100M", **font)
plt.ylabel("Runtime (seconds)", **font)
plt.title("Runtime for Gather and Scatter with medium sized filters", **font)
plt.legend()
plt.savefig('../plots/runtimeGatherScatter.eps', format='eps')
plt.close()
