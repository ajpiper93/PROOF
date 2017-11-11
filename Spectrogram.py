# -*- coding: utf-8 -*-
"""
Created on Fri May 12 13:44:19 2017

@author: Andrew Piper
"""

import math as m
#import cmath as cm
import numpy as  np
import matplotlib.pyplot as plt

fileI = "C:/Users/Admin/Desktop/Research/Proof/Intensity.txt"
fileAlphas = "C:/Users/Admin/Desktop/Research/Proof/Alpha.txt"
fileE = "C:/Users/Admin/Desktop/Research/Proof/Energy.txt"
filePhases = "C:/Users/Admin/Desktop/Research/Proof/Phases.txt"

""""""""""""""""""""""""""""""""""""""""""""""""
res= 0.5
fund = 1.5
emin = 10.5
pixels = 46  +   2
time = 10
hbar = 6.582119 * 10**-16
omega = fund/hbar * 10**-15
Pi = 3.1416
N = 200
tres = 0.1
""""""""""""""""""""""""""""""""""""""""""""""""

tau = []
z = [[] for n in range(pixels - 2)]

energy = lambda a: (emin + fund*a)

phase = lambda a: np.sin(16*( a/ float(pixels) - 1/2)) + 1
    
intensity = lambda a: np.exp( -((4*a/ float(pixels) ) - 2) ** 2)
    
def unwrap(data):
    for q in range(len(data)-1):
        while data[q] - data[q+1] > Pi:
            data[q + 1] += 2*Pi
        while data[q] - data[q+1] < -Pi:
            data[q + 1] -= 2*Pi
                
with(open(fileE, 'w')) as text:
    text.write(str("%.2f" % res) + "\n")
    text.write(str("%.2f" % emin) + "\n")
    text.write(str(pixels) + "\n")
    text.write(str("%.2f" % fund) + "\n")
    
phassss = []
blah = []

#random = np.random.rand(pixels)

pix = np.arange(float(pixels))

e = energy(pix[1:-1])
intensities = intensity(pix)
phases = phase(pix)

with (open(filePhases, 'w')) as text:
    for p in range(1, pixels - 1, 1):
        text.write(str("%.6f" %  phases[p]))
        if p < pixels - 2:
            text.write("\n")
    

with (open(fileI, 'w')) as text:
    for p in range(1, pixels - 1, 1):
        I = intensities[p]
        text.write(str("%.6f" % I))
        if p < pixels - 2:
            text.write("\n")
            
        Iplus = intensities[p+1]
        Iminus = intensities[p-1]
        phi = phases[p]
        phiplus = phases[p +1]
        phiminus = phases[p-1]
    
        gamma = m.sqrt( ( Iplus+Iminus - 2*m.sqrt(Iplus*Iminus)*m.cos(2*phi-phiplus-phiminus) ) / I )
        y = m.sqrt(Iplus) * m.sin(phi - phiplus) - m.sqrt(Iminus) * m.sin(phiminus - phi)
        x = m.sqrt(Iplus) * m.cos(phi - phiplus) - m.sqrt(Iminus) * m.cos(phiminus - phi)
        alpha = m.atan2(y,x)
        blah.append(alpha)
        for t in range(N):
            if p == 1 : tau.append(t*tres - tres*N/2)
            z[p-1].append( I + I*gamma*m.sin( omega*(t*tres)+alpha) )
""""""""""""""""""""""""""""""""""""""""""

maximum = max(max(z))

for i in range(len(z)):
    for j in range(len(z[i])):
        z[i][j] = z[i][j] / maximum

x, y = np.meshgrid(tau, e)
intent = np.array(z)

for q in range(len(z)):
    z[q] = np.fft.rfft(z[q])
    
for a in range(len(z)):
    for p in range(len(z[a])):
        if p != int(N*tres*omega/(2*Pi)):
            z[a][p] = 0;
            
for q in range(len(z)):
    z[q] = np.fft.irfft(z[q])

results = np.array(z)

for row in range(len(results)):
    norm = max(results[row])
    for l in range(len(results[row])):
        if norm == 0:
            results[row][l] = np.sign(norm)
        else:
            results[row][l] = results[row][l] / norm

alphas = []
col = 0
for t in range(len(results[0])):
    if results[0][t] > 0.999:
        col = t
        naught = col
        break
  
unwrap(blah)
'''
with (open(fileAlphas, 'w')) as text:
    for p in range(pixels):
        while True:
            if results[p][col - 1] > results[p][col]:
                col -= 1
                continue
            elif results[p][col + 1] > results[p][col]:
                col += 1
                continue
            else:
                break
        phase = - omega*col*tres + omega*naught*tres
            
        text.write(str("%.6f" % phase))
        if p < pixels -1:
            text.write("\n")
        alphas.append(phase)
'''
with (open(fileAlphas, 'w')) as text:
    for p in range(pixels-2):
        text.write(str("%.6f" % blah[p]))
        if p < pixels -3:
            text.write("\n")

plt.plot(np.diff(phases), 'r.')
#plt.plot(alphas)
#plt.plot(blah)

figure, axes = plt.subplots(nrows = 2)
im0 = axes[0].pcolormesh(x, y, intent)
im1 = axes[1].pcolormesh(x, y, results)
figure.colorbar(im0,ax = axes.ravel().tolist())
