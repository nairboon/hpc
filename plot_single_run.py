# -*- coding: utf-8 -*-
"""
Created on Sun Jun 28 15:21:43 2015

@author: nairboon
"""


import numpy as np
import matplotlib.pyplot as plt
import os

data = np.loadtxt('Output/times.log',delimiter=",",usecols=(2,3))

r = data[:,1]
plt.plot(r)

print np.mean(r)