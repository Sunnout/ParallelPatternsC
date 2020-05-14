"""
@authors: Ema Vieira - 50095, Andre Atalaia - 51910

"""

import pandas as pd 
import matplotlib.pyplot as plt


par_data = pd.read_csv("../output/output_t3_par.csv") 
par_data.columns = ['threads', 'size', 'test', 'time']


heavyScan_par = par_data[par_data['test'].str.contains("testScanLightWorker")]



#Plot
font = {'fontname':'Arial'}
plt.figure(1, figsize=(12,8))
plt.plot(heavyScan_par['size'], heavyScan_par['time'], color='firebrick', linewidth=1.5, marker='D', linestyle='-', label="Scan with heavy job")
plt.axis([None, None, None, None])
plt.xlabel("Input Size", **font)
plt.ylabel("Run Time (seconds)", **font)
plt.title("Stair Effect in Parallel Scan")
plt.legend()
plt.savefig('../plots/stairEffectScan.pdf', format='pdf')
plt.close()
