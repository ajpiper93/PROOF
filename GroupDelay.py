# -*- coding: utf-8 -*-
"""
Created on Wed Mar 29 11:09:14 2017

@author: Admin
"""

import matplotlib.pyplot as plt
import numpy as np
import statistics as s
import cmath as m

def unpack(fpath):
    with open(fpath) as text:
        i=0
        dat=[]
        r = text.readline()
        raw = text.readlines()
        for lines in raw:
            if i < len(raw):
                x = lines.split("\t")
                x[-1] = x[-1].rstrip()
                y = [float(num) for num in x]
                dat.append(y)
                i+=1
        return float(r), dat
        
def unwrap(data):
    for q in range(len(data)-1):
        while data[q] - data[q+1] > 3.14:
            data[q + 1] += 2*3.14
        while data[q] - data[q+1] < -3.14:
            data[q + 1] -= 2*3.14
              
def align(real,measured):
    best = 0
    bestR = 100000000
    for i in range(20000):
        R= 0
        for j in range(len(real)):
            measured[j] += (-100 + i/50)
            R += intensity[j]*(real[j] - measured[j])**2
            measured[j] -= (-100 + i/50)
        if (R < bestR):
            bestR = R
            best = i
    return (-100 + best/50)

n = 1
dat = []
R = [0]*n 
iRt = 0 
for i in range(n):
    R[i], temp = unpack("C:/Users/Admin/Desktop/Research/Proof/bestest" + str(i) + ".txt")
    dat.append(temp)
    iRt += 1/R[i]

Pi=3.14

intensity = []
energy = []
results = [[]for i in range(n)]
           
for q, value in enumerate(dat[0]):
    energy.append(value[0])
    intensity.append(value[1])
    for i in range(n):
        results[i].append(dat[i][q][2])

pixels = len(intensity);

for i in range(n):
    unwrap(results[i])

group_delay = np.diff(results[0])

phase = lambda a: np.sin(16*( a/ float(len(intensity)) - 1/2)) + 1
real_phases = phase(np.arange(len(intensity)))

#offset = group

real_GD = np.diff(real_phases)

print(R)
'''
for i in range(1,n):
    me = s.mean(results[i])
    for j, elem in enumerate(results[i]):
        results[i][j] -= me;

for i in range(len(results[0])):
    for j in range(len(results)):
        results[j][i] +=  offset[j]
'''

plt.plot(real_GD, label = 'real GD')
plt.plot(group_delay, label = 'PROOF GD')
#plt.plot(real_GD+group_delay, label = 'Difference')
plt.ylim(-1,1)
plt.legend()
