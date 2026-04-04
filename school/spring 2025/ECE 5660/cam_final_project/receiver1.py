import numpy as np
import math
import functions as myfunc
import matplotlib.pyplot as plt

Ts = 1      # symbol period
N = 4       # samples per symbol

alpha = 0.7   # Excess bandwidth
Lp = 12

srrc = myfunc.srrc1(alpha, N, Lp)

Omega0 = math.pi/2

LUT, Enorm = myfunc.getLUT()

# get signal
r = myfunc.readInputFile("sim1_2025")
r = np.array(r)

# demodulate
n = np.arange(r.size)
cosine = math.sqrt(2)*np.cos(Omega0*n)
sine = math.sqrt(2)*np.sin(Omega0*n)

I = r * cosine
Q = r * sine * (-1)

# matched filter
I_filtered = myfunc.matchedFilter(I, srrc)
Q_filtered = myfunc.matchedFilter(Q, srrc)

# downsample
x = []
y = []
cnt = N 
#for n in range(len(I_filtered)):
for n in range(Lp*4 - (N - 2), len(I_filtered) - Lp*4):
    cnt -= 1
    if cnt == 0:
        cnt = N
        x.append(I_filtered[n])
        y.append(Q_filtered[n])
        if (I_filtered[n] < 0.05 and I_filtered[n] > -0.05 and Q_filtered[n] < 0.05 and Q_filtered[n] > -0.05):
            print("It is 0 const point at index ", len(x), " which is time ", n)
        

# map to constellation
constx, consty, points = myfunc.mapToConstellation(x,y,LUT)
myfunc.drawKitties(points.tolist(), 130)

# plot
#plt.figure(1)
#plt.title("Downsampled Constellation Points")
#plt.scatter(x, y)
#plt.grid(True); plt.show(block=True)

print("Total number of points ", len(x))
