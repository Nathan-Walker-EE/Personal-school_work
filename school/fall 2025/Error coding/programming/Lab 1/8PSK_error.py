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

def bit_errors(a, b):
    return bin(a ^ b).count("1")

Eb = 1/3
p = .5
N = 8
d = 2
Es = 3 * Eb
ILUT, QLUT = sc.MPSK(N,Es)

maxerrorcount = 100
startSNRdB = 0
endSNRdB = 12
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
    sigma = np.sqrt(sigma2)
    errorcount = 0
    numsymbols = 0
    numbits = 0 
    biterrorcount = 0
    while(errorcount < maxerrorcount):
        numsymbols += 1
        numbits += math.log2(N)

        num = np.random.randint(0,8)
        Isymbol = ILUT[num]
        Qsymbol = QLUT[num]
        symbol = Isymbol + 1j*Qsymbol

        noisei = Es * np.random.normal(0, sigma)/1
        noiseq = Es * np.random.normal(0, sigma)/1
        noise = noisei + 1j*noiseq
        r = symbol + noise
        ri = np.real(r)
        rq = np.imag(r)

        shat = sc.PSK_dec(N, Es, ILUT, QLUT, ri, rq)

        if shat != num:
            errorcount += 1
            biterrorcount += bit_errors(shat, num)

    Psymerror[index] = errorcount/numsymbols
    pbiterror[index] = biterrorcount/numbits
    pactualerror[index] = 2/3 * Q_function(np.sin(np.pi/8) * np.sqrt(6*Eb/N0)) 
    
    index += 1

plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pbiterror, 'o-', label="Simulated Bit Error")
plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pactualerror, 's-', label="Theoretical Symbol Error")
plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), Psymerror, 'r--', label="Simulated Symbol Error")
plt.title('8PSK')
plt.xlabel('Eb/N0 (dB)')
plt.ylabel('Pb')
plt.legend()
plt.grid(True, which='both')
plt.ylim(10**-6,1)
plt.show()