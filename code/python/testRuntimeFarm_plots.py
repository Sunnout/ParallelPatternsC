#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data = pd.read_csv("../output/output_t4_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']

 
testFarmLightWorkerSmallNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmLightWorkerSmallNumberOfWorkers")]

testFarmLightWorkerMediumNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmLightWorkerMediumNumberOfWorkers")]

testFarmLightWorkerLargeNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmLightWorkerLargeNumberOfWorkers")]

testFarmHeavyWorkerSmallNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmHeavyWorkerSmallNumberOfWorkers")]

testFarmHeavyWorkerMediumNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmHeavyWorkerMediumNumberOfWorkers")]

testFarmHeavyWorkerLargeNumberOfWorkers_par = par_data[par_data['test'].str.contains("testFarmHeavyWorkerLargeNumberOfWorkers")]





plt.figure(1, figsize=(12,8))
plt.plot(testFarmLightWorkerSmallNumberOfWorkers_par['threads'], testFarmLightWorkerSmallNumberOfWorkers_par['time'], '-ro', label="testFarmLightWorkerSmallNumberOfWorkers")
plt.plot(testFarmLightWorkerMediumNumberOfWorkers_par['threads'], testFarmLightWorkerMediumNumberOfWorkers_par['time'], '-og', label="testFarmLightWorkerMediumNumberOfWorkersd")
plt.plot(testFarmLightWorkerLargeNumberOfWorkers_par['threads'], testFarmLightWorkerLargeNumberOfWorkers_par['time'], '-ob', label="testFarmLightWorkerLargeNumberOfWorkers")
plt.plot(testFarmHeavyWorkerSmallNumberOfWorkers_par['threads'], testFarmHeavyWorkerSmallNumberOfWorkers_par['time'], '-r', label="testFarmHeavyWorkerSmallNumberOfWorkers")
plt.plot(testFarmHeavyWorkerMediumNumberOfWorkers_par['threads'], testFarmHeavyWorkerMediumNumberOfWorkers_par['time'], '-g', label="testFarmHeavyWorkerMediumNumberOfWorkers")
plt.plot(testFarmHeavyWorkerLargeNumberOfWorkers_par['threads'], testFarmHeavyWorkerLargeNumberOfWorkers_par['time'], '-b', label="testFarmHeavyWorkerLargeNumberOfWorkers")


plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Million")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.title("Runtime for different number of Threads")
plt.show()
#plt.savefig('../plots/runtimefarm.png')
plt.close()



