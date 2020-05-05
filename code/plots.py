#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue May  5 14:42:32 2020

@author: emarodriguesvieira
"""
"""
Script to make plots
"""

import pandas as pd 
import matplotlib.pyplot as plt
import numpy as np



# Read data from file 'filename.csv' 
# (in the same directory that your python process is based)
# Control delimiters, rows, column names with read_csv (see later) 
data = pd.read_csv("output_t2_par.csv") 
data.set_index(['Threads', 'Size', 'Test', 'Time'])
par_data = pd.DataFrame(data)
par_data.columns = ['threads', 'size', 'test', 'time']
data = pd.read_csv("output_t2_seq.csv") 
seq_data = pd.DataFrame(data)
seq_data.columns = ['threads', 'size', 'test', 'time']


print(par_data[par_data['test'] == 'testScatterMediumFilter'])

print(par_data.loc[['test']])

