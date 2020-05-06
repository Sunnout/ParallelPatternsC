#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
par_data = pd.read_csv("output_t2_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']



testPipelineLightWorkersSmallNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersSmallNumberOfStations")]

testPipelineLightWorkersMediumNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersMediumNumberOfStations")]

testPipelineLightWorkersLargeNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersLargeNumberOfStations")]

testPipelineHeavyWorkersSmallNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersSmallNumberOfStations")]

testPipelineHeavyWorkersMediumNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersMediumNumberOfStations")]

testPipelineHeavyWorkersLargeNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersLargeNumberOfStations")]





plt.figure(1, figsize=(12,8))
plt.plot(testPipelineLightWorkersSmallNumberOfStations_par['threads'], testPipelineLightWorkersSmallNumberOfStations_par['time'], '-ro', label="testPipelineLightWorkersSmallNumberOfStations")
plt.plot(testPipelineLightWorkersMediumNumberOfStations_par['threads'], testPipelineLightWorkersMediumNumberOfStations_par['time'], '-og', label="testPipelineLightWorkersMediumNumberOfStations")
plt.plot(testPipelineLightWorkersLargeNumberOfStations_par['threads'], testPipelineLightWorkersLargeNumberOfStations_par['time'], '-ob', label="testPipelineLightWorkersLargeNumberOfStations")
plt.plot(testPipelineHeavyWorkersSmallNumberOfStations_par['threads'], testPipelineHeavyWorkersSmallNumberOfStations_par['time'], '-r', label="testPipelineHeavyWorkersSmallNumberOfStations")
plt.plot(testPipelineHeavyWorkersMediumNumberOfStations_par['threads'], testPipelineHeavyWorkersMediumNumberOfStations_par['time'], '-g', label="testPipelineHeavyWorkersMediumNumberOfStations")
plt.plot(testPipelineHeavyWorkersLargeNumberOfStations_par['threads'], testPipelineHeavyWorkersLargeNumberOfStations_par['time'], '-b', label="testPipelineHeavyWorkersLargeNumberOfStations")


plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n Input Size: 100 Million")
plt.ylabel("Runtime (seconds)")
plt.legend()
plt.title("Runtime for different number of Threads")
plt.show()
#plt.savefig('plots/test2_runtime.png')
plt.close()



