# -*- coding: utf-8 -*-


import numpy as np
import matplotlib.pyplot as plt
import os




def sorted_ls(path):
    mtime = lambda f: os.stat(os.path.join(path, f)).st_mtime
    dirs = [ name for name in os.listdir(path) if os.path.isdir(os.path.join(path, name)) ]
    return list(sorted( dirs, key=mtime,reverse=True))



    
def load_all(run):
    path = 'Experiment/%s' % run
    recent = sorted_ls(path)[0]
    path = "%s/%s" % (path,recent)
    keys = map(int,sorted_ls(path))
    
    bigarr = {}
    for k in keys:
        data = np.loadtxt('%s/%s/times.log' % (path,k),delimiter=",",usecols=(2,3))
        #print k,data        
        bigarr[k] = ( data[:,1])
        
        
    return (bigarr,sorted(keys))



def plot_all(ba):
    for key in ba:
        plt.plot(ba[key], label=key)
    plt.legend()
    
    

dirlist = sorted_ls('Experiment')

allsc = []
for ex in dirlist:
    arr, key = load_all(ex)
    allsc.append(arr)
    means = []
    for i in key:
        means.append(np.mean(arr[i]))
        
    plt.plot(key,means,marker='o', label=ex)
    print key,means

plt.legend()
plt.show()

#plt.waitforbuttonpress()






#
#
##print ba[]
#mlist = []
#for i in masterarray:
#    mlist.append(np.mean(ba[i]))
#    
#
#plt.plot(masterarray,mlist,marker='o')    
#plt.show()
#
#plt.waitforbuttonpress()

#plot_all(ba)

plt.show()

