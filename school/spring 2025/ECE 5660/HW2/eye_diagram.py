import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt

def srrc_pulse(N, Lp, Ts, alpha):
    p = np.zeros(2 * Lp + 1)
    for n in range(-Lp, Lp + 1):
        t = n / N  # Normalize n by samples per symbol
        if t == 0:
            p[n + Lp] = 1 / np.sqrt(N) * (1 + alpha * (4 / np.pi - 1))
        elif abs(t) == 1 / (4 * alpha):
            p[n + Lp] = (alpha / (np.sqrt(2 * N))) * \
                ( (1 + 2 / np.pi) * np.sin(np.pi / (4 * alpha)) +
                  (1 - 2 / np.pi) * np.cos(np.pi / (4 * alpha)) )
        else:
            numerator = np.sin(np.pi * (1 - alpha) * t) + \
                        4 * alpha * t * np.cos(np.pi * (1 + alpha) * t)
            denominator = (np.pi * t) * (1 - (4 * alpha * t) ** 2)
            p[n + Lp] = (1 / np.sqrt(N)) * (numerator / denominator)
    return p

a = 1 # PAM amplitude
problem = 6
if problem == 1:
    alpha = 1 # excess bandwidth
    N = 11 # samples per symbol
    Lp = 60 # SRRC truncation length
    Ts = 1 # symbol time
    T = Ts/N # sample time
    pulse = srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse
    rpulse = np.convolve(pulse,pulse)
    # peak at 2Lp
    plt.figure(1); plt.clf()
    plt.plot(pulse)
    plt.show()
    LUT1d = np.array([-3, -1, 1, 3])*a # 1-dimensional lookup table
    Nsym = 100 # number of symbols
    bits = np.zeros(Nsym, dtype=int)
    for i in range(0,Nsym):
        bits[i] = random.randint(0,4) # generate random bits {0,1}

    ampa = LUT1d[bits] # map the bits to {+1,-1} values
elif problem == 2:
    alpha = .5 # excess bandwidth
    N = 11 # samples per symbol
    Lp = 60 # SRRC truncation length
    Ts = 1 # symbol time
    T = Ts/N # sample time
    pulse = srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse
    rpulse = np.convolve(pulse,pulse)
    # peak at 2Lp
    plt.figure(1); plt.clf()
    plt.plot(pulse)
    plt.show()
    LUT1d = np.array([-3, -1, 1, 3])*a # 1-dimensional lookup table
    Nsym = 100 # number of symbols
    bits = np.zeros(Nsym, dtype=int)
    for i in range(0,Nsym):
        bits[i] = random.randint(0,4) # generate random bits {0,1}

    ampa = LUT1d[bits] # map the bits to {+1,-1} values
elif problem == 3:
    alpha = 1 # excess bandwidth
    N = 11 # samples per symbol
    Lp = 60 # SRRC truncation length
    Ts = 1 # symbol time
    T = Ts/N # sample time
    pulse = srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse
    rpulse = np.convolve(pulse,pulse)
    # peak at 2Lp
    plt.figure(1); plt.clf()
    plt.plot(pulse)
    plt.show()
    LUT1d = np.array([-3, -1, 1, 3])*a # 1-dimensional lookup table
    Nsym = 100 # number of symbols
    bits = np.zeros(Nsym, dtype=int)
    for i in range(0,Nsym):
        bits[i] = random.randint(0,4) # generate random bits {0,1}

    ampa = LUT1d[bits] # map the bits to {+1,-1} values
elif problem == 4:
    alpha = .5 # excess bandwidth
    N = 11 # samples per symbol
    Lp = 60 # SRRC truncation length
    Ts = 1 # symbol time
    T = Ts/N # sample time
    pulse = srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse
    rpulse = np.convolve(pulse,pulse)
    # peak at 2Lp
    plt.figure(1); plt.clf()
    plt.plot(pulse)
    plt.show()
    LUT1d = np.array([-3, -1, 1, 3])*a # 1-dimensional lookup table
    Nsym = 100 # number of symbols
    bits = np.zeros(Nsym, dtype=int)
    for i in range(0,Nsym):
        bits[i] = random.randint(0,4) # generate random bits {0,1}

    ampa = LUT1d[bits] # map the bits to {+1,-1} values
elif problem == 5:
    Lp = 60
    N = 11
    Ts = 1
    T = Ts/N
    pulse = np.ones(N)
    rpulse = np.convolve(pulse,pulse)
    plt.figure(1); plt.clf()
    plt.plot(pulse)
    plt.show()
    LUT1d = np.array([-1, 1])*a # 1-dimensional lookup table
    Nsym = 100 # number of symbols
    bits = np.zeros(Nsym, dtype=int)
    for i in range(0,Nsym):
        bits[i] = random.randint(0,1) # generate random bits {0,1}

    ampa = LUT1d[bits] # map the bits to {+1,-1} values

elif problem == 6:
    N = 12
    Ts = 1
    Lp = 60
    T = Ts/N
    pulse = np.ones(N)
    for i in range(int(np.floor(N/2)), N):
        pulse[i] = -1
    rpulse = np.convolve(pulse,pulse)
    plt.figure(1); plt.clf()
    plt.plot(pulse)
    plt.show()
    LUT1d = np.array([-1, 1])*a # 1-dimensional lookup table
    Nsym = 100 # number of symbols
    bits = np.zeros(Nsym, dtype=int)
    for i in range(0,Nsym):
        bits[i] = random.randint(0,1) # generate random bits {0,1}

    ampa = LUT1d[bits] # map the bits to {+1,-1} values


# for i in range(0,Nsym): upsampled[N*i] = ampa[i]
upsampled = np.zeros((N*Nsym,1))
upsampled[range(0,N*Nsym,N)] = ampa.reshape(Nsym,1)
plt.figure(2); plt.clf()
plt.stem(upsampled,linefmt= 'b',markerfmt= '.',basefmt= 'b-')
s = np.convolve(upsampled.reshape((N*Nsym,)),pulse) # the transmitted signal
plt.figure(3); plt.clf()
plt.plot(s)
plt.show()
x = np.convolve(s,pulse) # the matched filter
plt.figure(4); plt.clf()
plt.plot(x[:(2*Lp+1) + N*20])
for i in range(20):
    plt.plot(
        np.array([2*Lp + i*N,
        2*Lp + i*N]),#for eye diagram throw away transients at beginning and end
        np.array([-2,2]),color='gray',linewidth=0.25)
plt.show()
print('bits=',bits[:10])
# first peak at 2*Lp, then every N samples after that
offset = (2*Lp - np.floor(N/2)).astype(int)
# ˆ ˆ
# 1st correlation |
# peak |
# move to center
Nsymtoss = 2*np.ceil(Lp/N) # throw away symbols at the end
#Nsymtoss = 0
nc = (np.floor((len(x) - offset - Nsymtoss*N)/N)).astype(int) # number of points of signal to plot
xreshape = x[offset:offset + nc*N].reshape(nc,N)
plt.figure(5); plt.clf()
if N % 2 != 0:
    plt.plot(np.arange(-np.floor(N/2), np.floor(N/2)+1), xreshape.T,color='b',
    linewidth=0.25)
else:
    plt.plot(np.arange(-int(N/2), int(N/2)), xreshape.T,color='b',
    linewidth=0.25)
plt.show()

