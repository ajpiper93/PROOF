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

offset = [0]*n

for i in range(n):
    unwrap(results[i])

for i in range(n):
    zero = results[i][int(len(results[i])/2)]
    for j in range(len(results[i])):
        results[i][j] -= zero

'''
for i in range(1,n):
    me = s.mean(results[i])
    for j, elem in enumerate(results[i]):
        results[i][j] -= me;

for i in range(len(results[0])):
    for j in range(len(results)):
        results[j][i] +=  offset[j]
'''
    
avg = []
err = []

for j in range(len(results[0])):
    total = 0
    stotal = 0
    for i in range(n):
        total += results[i][j]/R[i]
    avg.append(total/iRt)
    for i in range(n):
        stotal += ((results[i][j] - avg[j])**2)/R[i]
    err.append((stotal**(1/2))/iRt)

print(R)

freq_real = []
freq_meas = []

for q in range(len(avg)):
    freq_meas.append(m.sqrt(intensity[q])*m.exp(avg[q]*1j))
    
time_meas = abs(np.fft.fftshift(np.fft.fft(freq_meas)))**2

fig1 = plt.figure(1)
ax1 = fig1.add_subplot(111)

for i in range(n):
    ax1.plot(energy,results[i], color= 'lightgray', linestyle = "--")

ax1.errorbar(energy,avg, yerr = err, linestyle = "-", marker = "", color = 'g', label = 'PROOF Phase')
ax1.set_xlabel('eV')
ax1.set_ylabel('radians', color='g')
ax1.tick_params('y',colors = 'g')

ax1.legend(loc=0,framealpha=0)

ax2 = ax1.twinx()
ax2.plot(energy,intensity, 'b-')
ax2.set_ybound(0,1.7)
ax2.set_ylabel('Intensity', color='b')
ax2.tick_params('y',colors = 'b')

fig1 = plt.figure(1)    

plt.title('Phases')

fig2 = plt.figure(2)
ax3 = fig2.add_subplot(111)

ax3.plot(time_meas, "g")
'''