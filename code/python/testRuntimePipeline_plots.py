"""
@authors: Ema Vieira - 50095, André Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data = pd.read_csv("../output/output_t2_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']


#TODO escolher graficos do pipeline

testPipelineLightWorkersSmallNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersSmallNumberOfStations")]
testPipelineLightWorkersMediumNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersMediumNumberOfStations")]
testPipelineLightWorkersLargeNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineLightWorkersLargeNumberOfStations")]
testPipelineHeavyWorkersSmallNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersSmallNumberOfStations")]
testPipelineHeavyWorkersMediumNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersMediumNumberOfStations")]
testPipelineHeavyWorkersLargeNumberOfStations_par = par_data[par_data['test'].str.contains("testPipelineHeavyWorkersLargeNumberOfStations")]


#Plot
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
plt.plot(testPipelineLightWorkersSmallNumberOfStations_par['threads'], testPipelineLightWorkersSmallNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-', label="testPipelineLightWorkersSmallNumberOfStations")
plt.plot(testPipelineLightWorkersMediumNumberOfStations_par['threads'], testPipelineLightWorkersMediumNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-', label="testPipelineLightWorkersMediumNumberOfStations")
plt.plot(testPipelineLightWorkersLargeNumberOfStations_par['threads'], testPipelineLightWorkersLargeNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-', label="testPipelineLightWorkersLargeNumberOfStations")
plt.plot(testPipelineHeavyWorkersSmallNumberOfStations_par['threads'], testPipelineHeavyWorkersSmallNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-',label="testPipelineHeavyWorkersSmallNumberOfStations")
plt.plot(testPipelineHeavyWorkersMediumNumberOfStations_par['threads'], testPipelineHeavyWorkersMediumNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-', label="testPipelineHeavyWorkersMediumNumberOfStations")
plt.plot(testPipelineHeavyWorkersLargeNumberOfStations_par['threads'], testPipelineHeavyWorkersLargeNumberOfStations_par['time'], color='darkorange', linewidth=2, marker='D', linestyle='-', label="testPipelineHeavyWorkersLargeNumberOfStations")
plt.axis([None, None, None, 80])
plt.ylim(bottom=0)
plt.xlabel("Number of Threads \n \n Input Size: 100M", **font)
plt.ylabel("Runtime (seconds)", **font)
plt.title("Runtime for Gather and Scatter with medium sized filters", **font)
plt.legend()
plt.savefig('../plots/runtimePipeline.eps', format='eps')
plt.close()

