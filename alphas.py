# -*- coding: utf-8 -*-
"""
Created on Wed Apr 26 14:21:51 2017

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
        
def unwrap(data):
    for q in range(len(data)-1):
        while data[q] - data[q+1] > 3.14:
            data[q + 1] += 2*3.14
        while data[q] - data[q+1] < -3.14:
            data[q + 1] -= 2*3.14
        
file1 = "C:/Users/Admin/Desktop/Research/Proof/Alpha.txt"
file2 = "C:/Users/Admin/Desktop/Research/Proof/alphas1.txt"


dat1 = unpack(file1)
dat2 = unpack(file2)


real = []
alphas = []

alphas.append(0)

for val, q in enumerate(dat1):
    real.append(q[0])

for val, q in enumerate(dat2):
    alphas.append(q[0])

#unwrap(alphas)
    
fig1 = plt.figure(1)
ax1 = fig1.add_subplot(111)

ax1.plot(real, label = 'real')
ax1.plot(alphas, label = 'measured')

#fig2 = plt.figure(2)
#ax2 = fig2.add_subplot(111)
#ax2.plot(alphas, label = 'measured')

fig1 = plt.figure(1)    
plt.ylabel('[alpha]')
plt.xlabel('[harmonic]')
plt.legend(loc=1,framealpha=0,title='V')