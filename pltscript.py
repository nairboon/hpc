# -*- coding: utf-8 -*-


import numpy as np
import matplotlib.pyplot as plt
import os
from ggplot import *



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
scenario_T2 = []
scenario_T2_mem = []
scenario_T12 = []
for ex in dirlist:
    s = ex.split("_")[0]
    arr, key = load_all(ex)
    allsc.append(arr)
    means = []

    if not scenarios.has_key(s):
        scenarios[s] = fig.add_subplot(1,1,pi)
        scenarios[s].set_yscale('log')
        scenarios[s].set_xscale('log')
        
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
    elif s == "T2":
        var = ex.split("_")[1]
        dim = var.split("x")
        m = means[0]
        x=int(dim[0])/2
        tp = m/(x)
        mem = float(1000*((4*x)+82)*8.0/1024.0/1024.0)
        print x, mem, tp
        scenario_T2_mem.append( (x,mem ))
        scenario_T2.append( (x,tp ))
    elif s == "T12":
        var = ex.split("_")[1]
        dim = var.split("x")
        m = means[0]
        x=int(dim[0])/24
        tp = m/(x)
        print x, tp
        scenario_T12.append( (x,tp ))
    else:
        scenarios[s].plot(key,means,marker='o', label=ex)
        scenarios[s].legend()
    #print key,means

from operator import itemgetter

sd =sorted(scenario_D, key=itemgetter(0))
x,y = zip(*sd)

print "Min time is at: ", min(sd,key=itemgetter(1))

scenarios["D"].plot(x,y,marker='o', label=ex)

sd =sorted(scenario_T2, key=itemgetter(0))
x,y = zip(*sd)

print "Min time is at: ", min(sd,key=itemgetter(1))
scenarios["T2"].set_yscale('linear')
scenarios["T2"].plot(x,y,marker='o', label=ex)
sd =sorted(scenario_T2_mem, key=itemgetter(0))
x,y = zip(*sd)
scenarios["T2"].plot(x,y,marker='x', label="mem")

sd =sorted(scenario_T12, key=itemgetter(0))
x,y = zip(*sd)

print "Min time is at: ", min(sd,key=itemgetter(1))
scenarios["T12"].set_yscale('linear')
scenarios["T12"].plot(x,y,marker='o', label=ex)
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

