# -*- coding: utf-8 -*-
"""
Created on Sat Jun 27 11:34:40 2015

@author: stephan
"""

import numpy as np
import matplotlib.pyplot as plt


data = np.loadtxt('/home/stephan/Documents/hpc1bproject/hpc/Experiment/2015-06-27-12-51-23/2/times.log',delimiter=",",usecols=(2,3))

print(data[:,1])


X = np.linspace(0,2*np.pi,100)
Y=np.sin(X)

plt.plot(data[:,1])


