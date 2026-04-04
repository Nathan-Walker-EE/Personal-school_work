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

    Psymerror[index] = errorcount/numsymbols
    pbiterror[index] = biterrorcount/numbits
    pactualerror[index] = Q_function((tau + 1)/sigma) * p + Q_function((1 - tau)/sigma) * (1-p)
    
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