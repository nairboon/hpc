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

fig = plt.figure()
    
allsc = []

scenarios = {}
pi = 0

scenario_D = []
scenario_D_y = []
for ex in dirlist:
    s = ex.split("_")[0]
    arr, key = load_all(ex)
    allsc.append(arr)
    means = []

    if not scenarios.has_key(s):
        scenarios[s] = fig.add_subplot(4,1,pi)
        #scenarios[s].set_yscale('log')

        pi+=1
        
    
    for i in key:
        means.append(np.mean(arr[i]))
    
    #scenarios[s].append(key,means,ex)    
    #p = fig.add_subplot(211)    
    if s == "D":
        var = ex.split("_")[1]
        dim = var.split("x")
        scenario_D.append( (int(dim[0]), means[0]))
        #scenario_D_y.append(means[0])
    else:
        scenarios[s].plot(key,means,marker='o', label=ex)
        scenarios[s].legend()
    #print key,means

from operator import itemgetter

sd =sorted(scenario_D, key=itemgetter(0))
x,y = zip(*sd)

scenarios["D"].plot(x,y,marker='o', label=ex)
#
#for i, s in enumerate(scenarios):
#    p = fig.add_subplot(i+1,1,1)
#    key,means,ex = scenarios[s]    
#    p.plot(key,means,marker='o', label=ex)




fig.show()

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

