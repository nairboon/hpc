# -*- coding: utf-8 -*-


import numpy as np
import matplotlib.pyplot as plt
import os




def sorted_ls(path):
    mtime = lambda f: os.stat(os.path.join(path, f)).st_mtime
    dirs = [ name for name in os.listdir(path) if os.path.isdir(os.path.join(path, name)) ]
    return list(sorted( dirs, key=mtime,reverse=True))



    
def load_all(keys):
    bigarr = {}
    for k in keys:
        data = np.loadtxt('Experiment/%s/%s/times.log' % (recent,k),delimiter=",",usecols=(2,3))
        #print k,data        
        bigarr[k] = ( data[:,1])
        
        
    return bigarr



def plot_all(ba):
    for key in ba:
        plt.plot(ba[key], label=key)
    plt.legend()
    plt.show()
    

recent = sorted_ls('Experiment')[0]


masterarray = [1,2,3,4,5,6,8]




ba = load_all(masterarray)

#print ba[]
mlist = []
for i in masterarray:
    mlist.append(np.mean(ba[i]))
    

plt.plot(mlist)    
plt.show()

plot_all(ba)


