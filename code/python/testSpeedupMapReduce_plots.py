"""
@authors: Ema Vieira - 50095, Andre Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data_0 = pd.read_csv("../output/output_t0_par.csv") 
par_data_0.columns = ['threads', 'size', 'test', 'time']
seq_data_0 = pd.read_csv("../output/output_t0_seq.csv") 
seq_data_0.columns = ['threads', 'size', 'test', 'time']

par_data_6 = pd.read_csv("../output/output_t6_par.csv") 
par_data_6.columns = ['threads', 'size', 'test', 'time']
seq_data_6 = pd.read_csv("../output/output_t6_seq.csv") 
seq_data_6.columns = ['threads', 'size', 'test', 'time']

par_data_5 = pd.read_csv("../output/output_t5_par.csv") 
par_data_5.columns = ['threads', 'size', 'test', 'time']
seq_data_7 = pd.read_csv("../output/output_t5_seq.csv") 
seq_data_7.columns = ['threads', 'size', 'test', 'time']


#Heavy Workers
Hmap_par = par_data_5[par_data_5['test'].str.contains("testMapHeavyWorker")]
Hreduce_par = par_data_6[par_data_6['test'].str.contains("testReduceHeavyWorker") & (par_data_6['threads'] == 32)]
Hmap_seq = seq_data_7[seq_data_7['test'].str.contains("testMapHeavyWorker")]
Hreduce_seq = seq_data_6[seq_data_6['test'].str.contains("testReduceHeavyWorker")]

#Light Workers
Lmap_par = par_data_0[par_data_0['test'].str.contains("testMapLightWorker") & (par_data_0['size'] == 200000000)]
Lreduce_par = par_data_0[par_data_0['test'].str.contains("testReduceLightWorker") & (par_data_0['size'] == 200000000)]
Lmap_seq = seq_data_0[seq_data_0['test'].str.contains("testMapLightWorker") & (par_data_0['size'] == 200000000)]
Lreduce_seq = seq_data_0[seq_data_0['test'].str.contains("testReduceLightWorker") & (par_data_0['size'] == 200000000)]


SLmap = Lmap_seq['time'].values/Lmap_par['time'].values
SHmap = Hmap_seq['time'].values/Hmap_par['time'].values

SLreduce = Lreduce_seq['time'].values/Lreduce_par['time'].values
SHreduce = Hreduce_seq['time'].values/Hreduce_par['time'].values
speedups = [SLmap[0], SHmap[0], SLreduce[0],SHreduce[0]]


#Plot
font = {'fontname':'Arial'}
teamColours = ['navajowhite','orange','lightgreen','green']
fig, ax = plt.subplots()
ax = plt.bar(width=0.7, x=["Map with\n light jobs", "Map with\n heavy jobs", "Reduce with\n light jobs", "Reduce with\n heavy jobs"], height=speedups, color=teamColours)
plt.title("Time Speedup with light jobs vs. heavy jobs", **font)
plt.axis([None, None, None, 30])
plt.yticks([])
xlocs, xlabs = plt.xticks()
xlocs=[0,1,2,3]
plt.xticks(xlocs, **font)
plt.xlabel("\n Threads: 32", **font)

for i, v in enumerate(speedups):
    plt.text(xlocs[i] -0.16, v + 0.4, str("{:.2f}".format(v)))
    
#plt.show()
plt.savefig('../plots/speedupMapReduce.pdf', format='pdf')
