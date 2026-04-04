import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt
import pulses as ps

def gen_Qbitstream(Nsym, N, num_ints, ILUT, QLUT):
    bits = np.zeros(Nsym, dtype=int)
    for i in range(0,Nsym):
        bits[i] = random.randint(0,num_ints) # generate random bits {0,1}
    ampI = ILUT[bits] # map the bits to {+1,-1} values
    ampQ = QLUT[bits] # map the bits to {+1,-1} values
    upsampledI = np.zeros((N*Nsym,1))
    upsampledQ = np.zeros((N*Nsym,1))
    upsampledI[range(0,N*Nsym,N)] = ampI.reshape(Nsym,1)
    upsampledQ[range(0,N*Nsym,N)] = ampQ.reshape(Nsym,1)
    return upsampledI, upsampledQ

show_everything = 0
a = 1
alpha = 1 # excess bandwidth
Nsym = 500
type = '_16QAM'
N = 11 # samples per symbol
Lp = 60 # SRRC truncation length
Ts = 1 # symbol time
T = Ts/N # sample time
pulse = ps.srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse

#from first quadrant to 4th is 0 1 2 3 in binary
if type == '_QPSK':
    ILUT = np.array([1, -1, -1, 1])*a #In phase LUT
    QLUT = np.array([1, 1, -1, -1])*a #Quadrature phase LUT
elif type == '_16QAM':
    ILUT = np.array([3, 1, -1, -3, -3, -1, 1, 3, 3, 1, -1, -3, -3, -1, 1, 3])*a
    QLUT = np.array([3, 3, 3, 3, 1, 1, 1, 1, -1, -1, -1, -1, -3, -3, -3, -3])*a

#generate upsampled bits for pulse shaping
Idata, Qdata = gen_Qbitstream(Nsym, N, 15, ILUT, QLUT)
if show_everything == 1:
    plt.figure(1); plt.clf()
    plt.stem(Idata,linefmt= 'b',markerfmt= '.',basefmt= 'b-')
    plt.figure(2); plt.clf()
    plt.stem(Qdata,linefmt= 'b',markerfmt= '.',basefmt= 'b-')
    plt.show()

# Pulse shape
#convolve upsampled bitstream with pulse
sI = np.convolve(Idata.reshape((N*Nsym,)), pulse)
sQ = np.convolve(Qdata.reshape((N*Nsym,)), pulse)
if show_everything == 1:
    plt.figure(3); plt.clf()
    plt.plot(sI)
    plt.figure(4); plt.clf()
    plt.plot(sQ)
    plt.show()

## end of transmitter

## start of receiver
# matched filter
x = np.convolve(sI, pulse)
y = np.convolve(sQ, pulse)
x = x[2*Lp:len(x)-2*Lp]
y = y[2*Lp:len(y)-2*Lp]

plt.figure(5); plt.clf()
plt.plot(x,y)
plt.xlabel('I(t)')
plt.ylabel('Q(t)')
plt.title('alpha = ' + str(alpha))
plt.savefig(str(alpha) + '_bandwidth_TJ' + type + '.png')
plt.show(block=True)
