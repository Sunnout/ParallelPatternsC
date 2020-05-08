"""
@authors: Ema Vieira - 50095, André Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


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



#Plot
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
plt.plot(testFarmHeavyWorkerSmallNumberOfWorkers_par['threads'], sS/testFarmHeavyWorkerSmallNumberOfWorkers_par['time'],color='darkorange', linewidth=2, marker='D', linestyle='-', label="Farm with 8 workers")
plt.plot(testFarmHeavyWorkerMediumNumberOfWorkers_par['threads'], sM/testFarmHeavyWorkerMediumNumberOfWorkers_par['time'],color='olivedrab', linewidth=2, marker='D', linestyle='-', label="Farm with 32 workers")
plt.plot(testFarmHeavyWorkerLargeNumberOfWorkers_par['threads'], sL/testFarmHeavyWorkerLargeNumberOfWorkers_par['time'], color='saddlebrown', linewidth=2, marker='D', linestyle='-', label="Farm with 64 workers")
plt.axis([None, None, None, 60])
plt.xlabel("Number of Threads \n \n Input Size: 20k", **font)
plt.ylabel("Time Speedup", **font)
plt.title("Time Speedup for Farm with heavy jobs and varying number of workers", **font)
plt.legend()
#plt.show()
plt.savefig('../plots/speedUpFarmHeavy.eps', format='eps')
plt.close()



