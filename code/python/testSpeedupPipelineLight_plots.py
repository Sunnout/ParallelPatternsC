"""
@authors: Ema Vieira - 50095, André Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data = pd.read_csv("../output/output_t2_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']

seq_data = pd.read_csv("../output/output_t2_seq.csv") 
seq_data.columns = ['threads', 'size', 'test', 'time']

#testPipelineLightWorkersSmallNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersSmallNumberOfStations") & (par_data['threads'] != 1)]
#testPipelineLightWorkersMediumNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersMediumNumberOfStations") & (par_data['threads'] != 1)]
testPipelineLightWorkersLargeNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersLargeNumberOfStations") & (par_data['threads'] != 1)]
"""
s = seq_data[seq_data['test'].str.contains("testPipelineLightWorkersSmallNumberOfStations")]
sPLS = [s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0]]

s = seq_data[seq_data['test'].str.contains("testPipelineLightWorkersMediumNumberOfStations")]
sPLM =  [s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0]]
"""
s = seq_data[seq_data['test'].str.contains("testPipelineLightWorkersLargeNumberOfStations")]
sPLL =  [s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0]]

#Plot
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
#plt.plot(testPipelineLightWorkersSmallNumberOfStations_par['threads'], sPLS/testPipelineLightWorkersSmallNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-', label="testPipelineLightWorkersSmallNumberOfStations")
#plt.plot(testPipelineLightWorkersMediumNumberOfStations_par['threads'], sPLM/testPipelineLightWorkersMediumNumberOfStations_par['time'], color='pink', linewidth=2, marker='D', linestyle='-', label="testPipelineLightWorkersMediumNumberOfStations")
plt.plot(testPipelineLightWorkersLargeNumberOfStations_par['threads'], sPLL/testPipelineLightWorkersLargeNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-', label="Pipeline with light jobs and 3k stations")
plt.axis([None, None, None, 4])
plt.xlabel("Number of Threads \n \n Input Size: 50k", **font)
plt.ylabel("Time Speedup", **font)
plt.title("Time Speedup of Pipeline with light jobs and a large number of stations", **font)
plt.legend()
plt.savefig('../plots/speedupPipelineLight.pdf', format='pdf')
plt.close()

