# -*- coding: utf-8 -*-


import numpy as np
import matplotlib.pyplot as plt
import os



def sorted_ls(path):
    mtime = lambda f: os.stat(os.path.join(path, f)).st_mtime
    dirs = [ name for name in os.listdir(path) if os.path.isdir(os.path.join(path, name)) ]
    return list(sorted( dirs, key=mtime,reverse=True))



    
def load_all(run):
    path = 'mirror/Experiment/%s' % run
    recent = sorted_ls(path)[0]
    path = "%s/%s" % (path,recent)
    keys = map(int,sorted_ls(path))
    
    bigarr = {}
    for k in keys:
        data = np.loadtxt('%s/%s/times.log' % (path,k),delimiter=",",usecols=(2,3))
        #print k,data        
        bigarr[k] = ( data[:,1])
        
        
    return (bigarr,sorted(keys))


dirlist = sorted_ls('mirror/Experiment')

    
allsc = []

scenarios = {}
pi = 0

scenario_D = []
scenario_T2 = []
scenario_T2_mem = []
scenario_T12 = []


fig = plt.figure()
ex="S_ss_b"
arr, key = load_all(ex)
means = []
speedup = []
optimal_scaling = []
optimal_speedup = []


for i in key:
    m=np.mean(arr[i])
    means.append(m)
    
    s =means[0]/m
    o = means[0]/i
    #print i, s
    speedup.append(s)
    optimal_scaling.append(o)
    
    n=i
    su = s
    p = ((1/su)-1)/( (1/n) - 1)    
    #p = ( ( n/su) -n)/(1-n)

    if p < 1:
        sp = 1/((1-p)+ (p/n))
    else:
        sp = 1
    
    print i,p, sp, s
    optimal_speedup.append(sp)




plt.figure(1)
plt.plot(key,means,marker='o', label="measured")

plt.plot(key,optimal_scaling, label="ideal")

plt.ylabel("Time per step [s]")
plt.xlabel("MPI Processes")



plt.yscale('log')
plt.xscale('log')

plt.title("Strong scaling")

plt.legend()
plt.savefig("strongscale.pdf", bbox_inches='tight')
plt.show()

plt.figure(2)
a = plt.plot(key,speedup,marker='o', label="measured")
plt.plot(key,key, label="linear")
plt.plot(key,optimal_speedup, label="predicted")

plt.title("Total Speedup")

#plt.ylim(0,900)

#ax2 = plt.twinx()

#ax2.plot(key,optimal_p, label="optimalsfdf")
#ax2.set_ylim(0,2)

plt.ylabel("Speedup")
plt.xlabel("MPI Processes")

plt.legend()
plt.savefig("speedup.pdf", bbox_inches='tight')