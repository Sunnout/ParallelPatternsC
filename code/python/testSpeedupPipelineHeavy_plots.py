"""
@authors: Ema Vieira - 50095, Andre Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data = pd.read_csv("../output/output_t9_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']

seq_data = pd.read_csv("../output/output_t9_seq.csv") 
seq_data.columns = ['threads', 'size', 'test', 'time']

testPipelineHeavyWorkersSmallNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersSmallNumberOfStations") & (par_data['threads'] != 1)]
testPipelineHeavyWorkersMediumNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersMediumNumberOfStations") & (par_data['threads'] != 1)]
testPipelineHeavyWorkersLargeNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersLargeNumberOfStations") & (par_data['threads'] != 1)]

s = seq_data[seq_data['test'].str.contains("testPipelineHeavyWorkersSmallNumberOfStations")]
sPHS =  [s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0]]

s = seq_data[seq_data['test'].str.contains("testPipelineHeavyWorkersMediumNumberOfStations")]
sPHM =  [s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0]]

s = seq_data[seq_data['test'].str.contains("testPipelineHeavyWorkersLargeNumberOfStations")]
sPHL =  [s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0],s['time'].values[0]]



#Plot
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
plt.plot(testPipelineHeavyWorkersSmallNumberOfStations_par['threads'], sPHS/testPipelineHeavyWorkersSmallNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-',label="Pipeline with heavy jobs and 8 stations")
plt.plot(testPipelineHeavyWorkersMediumNumberOfStations_par['threads'], sPHM/testPipelineHeavyWorkersMediumNumberOfStations_par['time'], color='green', linewidth=2, marker='D', linestyle='-', label="Pipeline with heavy jobs and 300 stations")
plt.plot(testPipelineHeavyWorkersLargeNumberOfStations_par['threads'], sPHL/testPipelineHeavyWorkersLargeNumberOfStations_par['time'], color='firebrick', linewidth=2, marker='D', linestyle='-', label="Pipeline with heavy jobs and 3k stations")
plt.axis([None, None, None, 1.05])
plt.xlabel("Number of Threads \n \n Input Size: 500", **font)
plt.ylabel("Time Speedup", **font)
plt.title("Time Speedup of Pipeline with heavy jobs", **font)
plt.legend()
plt.savefig('../plots/speedupPipelineHeavy.pdf', format='pdf')
plt.close()
