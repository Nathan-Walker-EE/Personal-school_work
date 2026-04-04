import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt
import pulses as ps
import funky as funk

zeta = 1/np.sqrt(2); Bn = 0.02; T = 1; kp = 2 ; k0 = 1
k1 = funk.compute_K1(Bn, T, zeta, kp, k0)*1.95
k2 = funk.compute_K2(Bn, T, zeta, kp, k0)*1.95
a = 1
alpha = 1 # excess bandwidth
N = 4 # samples per symbol
Lp = 12 # SRRC truncation length
Ts = 1 # symbol time
T = Ts/N # sample time
pulse = ps.srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse

M = 256
Mroot = math.floor(2**(8/2))
a = np.reshape(np.arange(-Mroot+1,Mroot,2),(2*8,1))
b = np.ones((Mroot,1))
LUT = np.hstack((np.kron(a,b), np.kron(b,a)))
Enorm = np.sum(LUT ** 2) / (2**8)
LUT = LUT/math.sqrt(Enorm)

## start of receiver
r = funk.readInputFile("sim1_2025")
print(len(r))
r = np.array(r)
n = np.arange(r.size)
cosine = math.sqrt(2)*np.cos(math.pi*n/2)
sine = math.sqrt(2)*np.sin(math.pi*n/2)

I = r * cosine
Q = -r * sine

# matched filter
If = np.convolve(pulse, I)
Qf = np.convolve(pulse, Q)
x = []
y = []

cnt = N
bits_received = []
x1 = 0
x2 = 0
x3 = 0
x4 = 0
theta = 0

for i in range(Lp*4 - (N-2), len(If) - 4 * Lp):
    cnt = cnt-1
    if(cnt == 0):
        cnt = N
        x.append(If[i])
        y.append(Qf[i])

funk.plot_scatter(x,y)
constx, consty, points = funk.mapToConstellation(x,y,LUT)
#sim 1: 130
#sim 2: 
funk.drawKitties(points.tolist(), 130, 'sim 1')
