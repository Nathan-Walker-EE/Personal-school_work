import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt
import signal_constellation as sc
import scipy.stats as stats
from scipy.stats import norm
from scipy.special import erfc

def Q_function(x):
    return 0.5 * erfc(x / np.sqrt(2))

type = 'PAM'
Eb = 1
N = 2
d = 2
if type == 'PAM':
    Es = (N**2 - 1)/3 * Eb
    LUT = sc.MPAM(N,Es)
elif type == 'QAM':
    Es = Eb * math.log2(N)
    ILUT, QLUT = sc.MQAM(N,Eb)
elif type == 'QPSK':
    Es = 1
    Eb = Es / math.log2(N)
    dpsk = 2 * Eb*math.sin(np.pi/8)
    ILUT, QLUT = sc.MPSK(N,Es)
elif type == 'CCITT':
    Es = 1
    Eb = Es / math.log2(N)
    ILUT, QLUT = sc.CCITT(N,Es)
    d_min = sc.compute_d_min(ILUT, QLUT)

# plt.scatter(ILUT, QLUT)
# plt.show()
maxerrorcount = 100
startSNRdB = 0
endSNRdB = 11
SNRdBstep = 1
totalsteps = int((endSNRdB - startSNRdB)/SNRdBstep)
Psymerror = np.zeros(totalsteps)
pbiterror = np.zeros(totalsteps)
pactualerror = np.zeros(totalsteps)
index = 0

for SNRdB in np.arange(startSNRdB, endSNRdB, SNRdBstep):
    print(SNRdB)
    N0 = Eb/(10**(SNRdB/10))
    sigma2 = N0/2
    #print(sigma2)
    sigma = np.sqrt(sigma2)
    #print(sigma)
    errorcount = 0
    numsymbols = 0
    numbits = 0 
    biterrorcount = 0
    while(errorcount < maxerrorcount):
        numsymbols += 1
        #print(numsymbols, errorcount)
        numbits += math.log2(N)

        Num = random.randint(0,N-1) # generate random bits {0,1} 
        bits_in = bin(Num)[2:]
        if type == 'PAM':
            symbol = LUT[Num]
        elif type == 'QAM':
            Is = ILUT[Num]
            Qs = QLUT[Num]
        elif type == 'QPSK' or type == 'CCITT':
            Is = ILUT[Num]
            Qs = QLUT[Num]

        if type == 'PAM':
            noise = Es * np.random.normal(0, sigma)
            r = symbol + noise
        elif type == 'QAM':
            noisex = Eb * np.random.normal(0, sigma)
            noisey = Eb * np.random.normal(0, sigma)
            rx = Is + noisex
            ry = Qs + noisey
        elif type == 'QPSK':
            noisex = sigma * np.random.randn()
            noisey = sigma * np.random.randn()
            rx = Is + noisex
            ry = Qs + noisey
        elif type == 'CCITT':
            noisex = 2.5*np.random.normal(0, sigma)
            noisey = 2.5*np.random.normal(0, sigma)
            rx = Is + noisex
            ry = Qs + noisey

        if type == 'PAM':
            shat = sc.PAM_dec(N, Es, r)
            #print(symbol, r, shat)
        elif type == 'QAM':
            rinteger = sc.QAM_dec(N, Es, ILUT, QLUT, rx, ry)
            rbits = bin(rinteger)[2:]
        elif type == 'QPSK' or type == 'CCITT':
            rinteger = sc.PSK_dec(N, Es, ILUT, QLUT, rx, ry)
            rbits = bin(rinteger)[2:]
        #print(Num, rinteger)
        #print("SNR:", SNRdB, "symbol:", symbol, "noise:", noise, "shat:", shat)
        if type == 'PAM':
            if shat != symbol:
                errorcount += 1
                biterrorcount += 1
        elif type == 'QAM':
            if rbits != bits_in:
                errorcount += 1
                biterrorcount += np.sum([b1 != b2 for b1, b2 in zip(bits_in, rbits)])
        elif type == 'QPSK' or type == 'CCITT':
            if rbits != bits_in:
                errorcount += 1
                biterrorcount += np.sum([b1 != b2 for b1, b2 in zip(bits_in, rbits)])

    #print(errorcount, numsymbols)
    Psymerror[index] = errorcount/numsymbols
    print(Psymerror)
    pbiterror[index] = biterrorcount/numbits
    if type == 'PAM':
        pactualerror[index] = Q_function(d/(2 * sigma))
    elif type == 'QAM':
        if N == 4:
            pactualerror[index] = 2*Q_function(d/(2*sigma)) - (Q_function(d/(2*sigma)))**2
    elif type == 'QPSK':
        if N == 8:
            theta = np.pi / N
            pactualerror[index] = 2 * Q_function(np.sqrt(6 * 10**(SNRdB/10)) * np.sin(theta))
    elif type == 'CCITT':
        if N == 8:
            pactualerror[index] = 30 * Q_function(np.sqrt(6 * 10 ** (SNRdB / 22)) * (d_min / 2))
    #print(pbiterror[index])
    index += 1

plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pbiterror, 'o-', label="Simulated Bit Error")
plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pactualerror, 's-', label="Theoretical Symbol Error")
plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), Psymerror, 'r--', label="Simulated Symbol Error")
plt.title(type)
plt.xlabel('Eb/N0 (dB)')
plt.ylabel('Pb')
plt.legend()
plt.grid(True, which='both')
plt.ylim(10**-6,1)
plt.show()