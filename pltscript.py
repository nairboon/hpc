# -*- coding: utf-8 -*-
"""
Created on Sat Jun 27 11:34:40 2015

@author: stephan
"""

import numpy as np
import matplotlib.pyplot as plt


import os,time

import os

def sorted_ls(path):
    mtime = lambda f: os.stat(os.path.join(path, f)).st_mtime
    return list(sorted(os.listdir(path), key=mtime,reverse=True))

recent = sorted_ls('Experiment')[0]

    
def load_all(keys):
    bigarr = {}
    for k in keys:
        data = np.loadtxt('Experiment/%s/%s/times.log' % (recent,k),delimiter=",",usecols=(2,3))
        #print k,data        
        bigarr[k] = ( data[:,1])
        
        
    return bigarr



masterarray = [2,3,4,5,6,8]


def plot_all(ba):
    for key in ba:
        plt.plot(ba[key], label=key)
    plt.legend()
    plt.show()

ba = load_all(masterarray)

#print ba[]
list = []
for i in masterarray:
    list.append(np.mean(ba[i]))
    

plt.plot(list)    
plt.show()

plot_all(ba)


