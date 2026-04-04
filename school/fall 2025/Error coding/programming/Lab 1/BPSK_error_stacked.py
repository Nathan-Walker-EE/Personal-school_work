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
p = .1
probs = [p, 1-p]
N = 2
d = 2
Es = (N**2 - 1)/3 * Eb
LUT = sc.MPAM(N,Es)

maxerrorcount = 10
startSNRdB = 0
endSNRdB = 10
SNRdBstep = 1
totalsteps = int((endSNRdB - startSNRdB)/SNRdBstep)
Psymerror = np.zeros(totalsteps)
pbiterror = np.zeros(totalsteps)
pbiterror5 = np.zeros(totalsteps)
pbiterror25 = np.zeros(totalsteps)
pbiterror1 = np.zeros(totalsteps)
pactualerror = np.zeros(totalsteps)
index = 0
ps = [.5, .25, .1]

for pindex in np.arange(0,3,1):
    p = ps[pindex]
    probs = [p, 1-p]
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
        tau = sigma2/(2 * math.sqrt(Eb))
        tau = tau * math.log(probs[0]/probs[1])
        while(errorcount < maxerrorcount):
            numsymbols += 1
            numbits += math.log2(N)

            bitstream = int(np.random.choice([0, 1], size=1, p=probs))
            symbol = LUT[bitstream]

            noise = Es * np.random.normal(0, sigma)
            r = symbol + noise

            if r > tau:
                shat = 1
            else:
                shat = -1

            if shat != symbol:
                errorcount += 1
                biterrorcount += 1

        if pindex == 0:
            pbiterror5[index] = biterrorcount/numbits
        if pindex == 1:
            pbiterror25[index] = biterrorcount/numbits
        if pindex == 2:
            pbiterror1[index] = biterrorcount/numbits
        
        index += 1

plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pbiterror5, 'o-', label="Simulated Bit Error p = .5")
plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pbiterror25, 's-', label="Simulated Bit Error p = .25")
plt.semilogy(np.arange(startSNRdB,endSNRdB,SNRdBstep), pbiterror1, 'r--', label="Simulated Bit Error p = .1")
plt.title('BPSK stacked error')
plt.xlabel('Eb/N0 (dB)')
plt.ylabel('Pb')
plt.legend()
plt.grid(True, which='both')
plt.ylim(10**-6,1)
plt.show()