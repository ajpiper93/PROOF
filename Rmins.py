# -*- coding: utf-8 -*-
"""
Created on Wed Mar 22 11:24:02 2017

@author: Admin
"""
import matplotlib.pyplot as plt

def unpack(fpath):
    with open(fpath) as text:
        i=0
        dat=[]
        raw = text.readlines()
        for lines in raw:
            if i < len(raw)-1:
                x = lines.split("\t")
                x[-1] = x[-1].rstrip()
                y = [float(num) for num in x]
                dat.append(y)
                i+=1
        return dat
        
file1 = "C:/Users/Admin/Desktop/Research/Proof/Rmins.txt"

dat1 = unpack(file1)

Rmin = []
R_avg = []
R_sd = []
 
for val, q in enumerate(dat1):
    Rmin.append(q[0])
    R_avg.append(q[1])
    R_sd.append(q[2]) 
    
print(Rmin[-1])    
fig1 = plt.figure(1)
ax1 = fig1.add_subplot(111)

#ax1.plot(R_sd, label = 'Rsd')
#ax1.plot(R_avg, label = 'Ravg')
ax1.plot(Rmin, label = 'Rmin', color = 'k', linewidth = 2)
  
ax1.set_ylabel('[R]')
ax1.set_yscale('log')
ax1.set_xlabel('[Iter]')
plt.title('R vs Iteration')
plt.legend(loc=1,framealpha=0,title='V')