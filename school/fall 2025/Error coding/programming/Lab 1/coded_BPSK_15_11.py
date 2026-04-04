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

Eb = 1
p = .5
probs = [p, 1-p]
N = 2
d = 2
Es = (N**2 - 1)/3 * Eb
LUT = sc.MPAM(N,Es)

P = np.array([
        [1, 0, 0, 1],
        [1, 1, 0, 0],
        [1, 0, 1, 0],
        [0, 1, 0, 1],
        [0, 1, 1, 0],
        [0, 0, 1, 1],
        [1, 1, 1, 0],
        [1, 1, 0, 1],
        [1, 0, 1, 1],
        [0, 1, 1, 1],
        [1, 1, 1, 1],
    ])
G = np.hstack((np.eye(11), P))
H = np.eye(4)
H = np.hstack((P.T, np.eye(4)))
print((G @ H.T)%2)

maxerrorcount = 50
startSNRdB = 0
endSNRdB = 10
SNRdBstep = 1
totalsteps = int((endSNRdB - startSNRdB)/SNRdBstep)
Psymerror = np.zeros(totalsteps)
pbiterror = np.zeros(totalsteps)
pactualerror = np.zeros(totalsteps)
index = 0
bitstream = [0, 0, 0, 0]
received_bits = [0, 0, 0, 0]

for SNRdB in np.arange(startSNRdB, endSNRdB, SNRdBstep):
    print(SNRdB)
    N0 = Eb/(10**(SNRdB/10))
    R = 11/15
    Ec = Eb*R
    sigma2 = N0/2
    sigma = np.sqrt(sigma2)
    errorcount = 0
    numsymbols = 0
    numbits = 0 
    biterrorcount = 0
    p = Q_function(np.sqrt(2*Ec/N0))
    while(errorcount < maxerrorcount):
        numsymbols += 11
        numbits += 11

        bitstream = np.random.choice([0,1], size=11, p=probs)
        C = (bitstream @ G)%2
        #print(C)

        flips = (np.random.rand(len(C)) < p).astype(int)
        rx = C.astype(int) ^ flips

        s = (rx @ H.T) % 2
        correction = np.zeros(15, dtype=int)
        if np.any(s):  # nonzero syndrome => single-bit error
            #print(rx, C, s)
            # find which column of H matches syndrome
            for j in range(H.shape[1]):
                if np.all(H[:, j] == s):
                    correction[j] = 1
                    break
        chat = (rx + correction) % 2

        received_bits = chat[:11]
        # print(bitstream, C, flips, rx, s, correction, chat, received_bits)
        for i in range(0,11):
            if received_bits[i] != bitstream[i]:
                errorcount +=1
                biterrorcount +=1

    Psymerror[index] = errorcount/numsymbols
    pbiterror[index] = biterrorcount/numbits
    pactualerror[index] = Q_function((1)/sigma) * p + Q_function((1)/sigma) * (1-p)
    
    index += 1

plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pbiterror, 'o-', label="Simulated Bit Error")
plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pactualerror, 's-', label="Theoretical Symbol Error")
plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), Psymerror, 'r--', label="Simulated Symbol Error")
plt.title('BPSK p = .1')
plt.xlabel('Eb/N0 (dB)')
plt.ylabel('Pb')
plt.legend()
plt.grid(True, which='both')
plt.ylim(10**-6,1)
plt.show()