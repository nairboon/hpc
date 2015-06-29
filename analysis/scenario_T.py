# -*- coding: utf-8 -*-


import numpy as np
import matplotlib.pyplot as plt
import os
from ggplot import *
from pandas import *



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
        #data = np.loadtxt('%s/%s/times.log' % (path,k),delimiter=",",usecols=(3,4,5))
        fp = '%s/%s/times.log' % (path,k)
        print "parse", fp
        p = read_csv(fp,names=["seq","T","dt","time","VmSize","VmRSS","outnum"],header=0, index_col=0)
        #print p
        #print k,data        
        bigarr[k] = p
        
        
    return (bigarr,sorted(keys))



    

dirlist = sorted_ls('mirror/Experiment')

    
allsc = []

scenarios = {}
pi = 0


fig = plt.figure()

scenario_T2 = []
scenario_T2_mean = []
scenario_T2_mem = []
scenario_T2_vmsize = []
scenario_T2_rss = []



scenario_T12 = []
scenario_T12_mean = []
scenario_T12_mem = []
scenario_T12_vmsize = []
scenario_T12_rss = []

plotT2 = fig.add_subplot(2,1,1)
plotT12 = fig.add_subplot(2,1,2)

def memf(x):
    if x < 1000:
        m= 1000*((4*x)+82)
    else:
        m = 1000*16 + (x*(4*1000)+66) 
        
    return float(m*8.0/1024.0/1024.0)
    
    
for ex in dirlist:
    s = ex.split("_")[0]
    



    
    

    
    #scenarios[s].append(key,means,ex)    
    #p = fig.add_subplot(211)    

    if s == "T2":
        arr, key = load_all(ex)
        allsc.append(arr)
    
        means = arr[2]["time"].mean() 
        rss =arr[2]["VmRSS"].mean() 
        vmsize =arr[2]["VmSize"].mean()
        
        print means
        #print arr
        var = ex.split("_")[1]
        dim = var.split("x")
        m = means
        
        
        x=int(dim[0])/2
        tp = m/(x)#*1000*100
        mem = memf(x)
        #print x, mem, tp
        scenario_T2_vmsize.append( vmsize/1024.0 )
        scenario_T2_rss.append( rss/1024.0 )

        scenario_T2_mem.append( mem )
        scenario_T2_mean.append( tp )
        scenario_T2.append( x)
    elif s == "T12":
        arr, key = load_all(ex)
        allsc.append(arr)
        means = arr[24]["time"].mean() 
        rss =arr[24]["VmRSS"].mean() 
        vmsize =arr[24]["VmSize"].mean()
        
        var = ex.split("_")[1]
        dim = var.split("x")
        m = means
        x=int(dim[0])/24
        tp = m/(x)
        mem = memf(x)
        #rint x, tp
        
        scenario_T12_mem.append( mem )
        scenario_T12_mean.append( tp )
        scenario_T12_vmsize.append( vmsize/1024.0 )
        scenario_T12_rss.append( rss/1024.0 )
        scenario_T12.append( x)
        
    else:
        continue
    #print key,means







df = DataFrame(data = zip(scenario_T2,scenario_T2_mean,scenario_T2_mem,scenario_T2_vmsize,scenario_T2_rss), columns=['xw', 't/mp','theoretical memory',"VmSize","RSS"])
Sorted = df.sort(['xw'], ascending=True)

Sorted.index = Sorted['xw']
Sorted.reset_index(drop=True)
Sorted = Sorted.drop('xw',1)

ax = Sorted.plot(secondary_y=["t/mp"],  ax=plotT2)
ax.set_ylabel('Usage in MB')
ax.set_xlabel('Grid width')

ax.right_ax.set_ylabel('time per 1000 cells')
ax.axhline(y=30)

m = df['xw'][df['t/mp'] == df['t/mp'].min()].values[0]

print "Min T2 runtime at: ", m




df = DataFrame(data = zip(scenario_T12,scenario_T12_mean,scenario_T12_mem,scenario_T12_vmsize,scenario_T12_rss), columns=['xw', 't/mp','theoretical memory',"VmSize","RSS"])
Sorted = df.sort(['xw'], ascending=True)

Sorted.index = Sorted['xw']
Sorted.reset_index(drop=True)
Sorted = Sorted.drop('xw',1)

ax = Sorted.plot(secondary_y=["t/mp"],  ax=plotT12)
ax.set_ylabel('Usage in MB')
ax.set_xlabel('Grid width')

ax.right_ax.set_ylabel('time per 1000 cells')
ax.axhline(y=30)

m = df['xw'][df['t/mp'] == df['t/mp'].min()].values[0]

print "Min T12 runtime at: ", m

