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
    return upsampledI, upsampledQ, bits

#variables for plotting various things
show_everything = 1
phase_traj = 1
eye_diag = 1
power_spec = 1

type = '_8QAM'
a = 1
alpha = 1 # excess bandwidth
N = 11 # samples per symbol
Lp = 60 # SRRC truncation length
Ts = 1 # symbol time
T = Ts/N # sample time
pulse = ps.srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse

#from first quadrant to 4th is 0 1 2 3 in binary
if type == '_QPSK':
    ILUT = np.array([1, -1, -1, 1])*a #In phase LUT
    QLUT = np.array([1, 1, -1, -1])*a #Quadrature phase LUT
elif type == '_8QAM':
    ILUT = np.array([3, 1, -1, -3, -3, -1, 1, 3])*a
    QLUT = np.array([1, 1, 1, 1, -1, -1, -1, -1])*a
elif type == '_16QAM':
    ILUT = np.array([3, 1, -1, -3, -3, -1, 1, 3, 3, 1, -1, -3, -3, -1, 1, 3])*a
    QLUT = np.array([3, 3, 3, 3, 1, 1, 1, 1, -1, -1, -1, -1, -3, -3, -3, -3])*a
Nsym = 1000

#generate upsampled bits for pulse shaping
Idata, Qdata, bits = gen_Qbitstream(Nsym, N, 7, ILUT, QLUT)
if show_everything == 1:
    plt.figure(1); plt.clf()
    plt.stem(Idata,linefmt= 'b',markerfmt= '.',basefmt= 'b-')
    plt.figure(2); plt.clf()
    plt.stem(Qdata,linefmt= 'b',markerfmt= '.',basefmt= 'b-')
    plt.show()


# Pulse shape
#convolve upsampled bitstream with pulse
I = np.convolve(Idata.reshape((N*Nsym,)), pulse)
Q = np.convolve(Qdata.reshape((N*Nsym,)), pulse)
if show_everything == 1:
    plt.figure(3); plt.clf()
    plt.plot(I)
    plt.figure(4); plt.clf()
    plt.plot(Q)
    plt.show()
if power_spec == 1:
    f = np.linspace(-.5, .5, 2*len(I)-1, endpoint=False)
    plt.plot( f,10*np.log10(np.abs(np.fft.fftshift( np.fft.fft(
    np.correlate(I,I,'full'))))**2))
    plt.xlabel('frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.title('transmitter I(t)')
    plt.savefig('transmitter_IT_ps.png')
    plt.show()


#multiply by cos scaled by np.sqrt(2)
fs = 1
fc = .35
n = np.arange(0, len(I))
sI = I * np.sqrt(2) * np.cos(2*np.pi*fc*n/fs)
sQ = Q * np.sqrt(2) * np.sin(2*np.pi*fc*n/fs)
if show_everything == 1:
    plt.figure(5); plt.clf()
    plt.plot(sI)
    plt.figure(6); plt.clf()
    plt.plot(sQ)
    plt.show()

#sum both signals
S = sI - sQ
if show_everything == 1:
    plt.figure(7); plt.clf()
    plt.plot(S)
    plt.show()
if power_spec == 1:
    f = np.linspace(-.5, .5, 2*len(S)-1, endpoint=False)
    plt.plot( f,10*np.log10(np.abs(np.fft.fftshift( np.fft.fft(
    np.correlate(S,S,'full'))))**2))
    plt.xlabel('frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.title('S(t)')
    plt.savefig('transmitter_St_ps.png')
    plt.show()

## end of transmitter

## start of receiver
# multiply by np.sqrt(2)
rI = S * np.sqrt(2) * np.cos(2*np.pi*n*fc/fs)
rQ = S * -np.sqrt(2) * np.sin(2*np.pi*n*fc/fs)
# matched filter
x = np.convolve(pulse, rI)
y = np.convolve(pulse, rQ)
if eye_diag == 1:
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
    plt.title('Eye diagram')
    plt.savefig(str(alpha) + '_eyediagx.png')
    plt.show()
    
    offset = (2*Lp - np.floor(N/2)).astype(int)
    # ˆ ˆ
    # 1st correlation |
    # peak |
    # move to center
    Nsymtoss = 2*np.ceil(Lp/N) # throw away symbols at the end
    #Nsymtoss = 0
    nc = (np.floor((len(y) - offset - Nsymtoss*N)/N)).astype(int) # number of points of signal to plot
    xreshape = y[offset:offset + nc*N].reshape(nc,N)
    plt.figure(5); plt.clf()
    if N % 2 != 0:
        plt.plot(np.arange(-np.floor(N/2), np.floor(N/2)+1), xreshape.T,color='b',
        linewidth=0.25)
    else:
        plt.plot(np.arange(-int(N/2), int(N/2)), xreshape.T,color='b',
        linewidth=0.25)
    plt.title('Eye diagram')
    plt.savefig(str(alpha) + '_eyediagy.png')
    plt.show()
    
if power_spec == 1:
    f = np.linspace(-.5, .5, 2*len(x) - 1, endpoint=False)
    plt.plot( f,10*np.log10(np.abs(np.fft.fftshift( np.fft.fft(
    np.correlate(x,x,'full'))))**2))
    plt.xlabel('frequency (Hz)')
    plt.ylabel('Magnitude (dB)')
    plt.title('receiver I(t)')
    plt.savefig('receiver_I(t)_ps.png')
    plt.show()
# sample time
xt = x[120:len(x)-120]
yt = y[120:len(y)-120]
if phase_traj == 1:
    plt.figure(20); plt.clf()
    plt.plot(x,y)
    plt.xlabel('I(t)')
    plt.ylabel('Q(t)')
    plt.title('alpha = ' + str(alpha))
    plt.savefig(str(alpha) + '_P3_PTJ.png')
    
if show_everything == 1:
    plt.figure(8); plt.clf()
    plt.plot(I)
    plt.figure(9); plt.clf()
    plt.plot(xt)
    plt.show()
xn = xt[::N]
yn = yt[::N]
plt.scatter(xn, yn, color='b', marker='o')
plt.xlabel('Inphase')
plt.ylabel('Quadrature Phase')
plt.title('received signal constellation')
plt.savefig('received_signal_constellation.png')
plt.show()
received_bits = np.zeros(len(xn), 'int')
ILUT = np.array([3, 1, -1, -3, -3, -1, 1, 3])*a
QLUT = np.array([1, 1, 1, 1, -1, -1, -1, -1])*a
for i in range(len(xn)):
    if yn[i] > 0:
        if xn[i] > 2 and xn[i] < 4:
            received_bits[i] = 0
        elif xn[i] > 0 and xn[i] < 2:
            received_bits[i] = 1
        elif xn[i] < 0 and xn[i] > -2:
            received_bits[i] = 2
        elif xn[i] > -4 and xn[i] < -2:
            received_bits[i] = 3
    elif yn[i] < 0:
        if xn[i] > 2 and xn[i] < 4:
            received_bits[i] = 7
        elif xn[i] > 0 and xn[i] < 2:
            received_bits[i] = 6
        elif xn[i] < 0 and xn[i] > -2:
            received_bits[i] = 5
        elif xn[i] > -4 and xn[i] < -2:
            received_bits[i] = 4


show_everything = 1
if show_everything == 1:
    plt.figure(10); plt.clf()
    plt.stem(received_bits,linefmt= 'b',markerfmt= '.',basefmt= 'b-')
    plt.xlabel('samples')
    plt.ylabel('bits in integer')
    plt.title('received bits')
    plt.savefig('received_bits.png')
    plt.show()
    plt.figure(11); plt.clf()
    plt.stem(bits,linefmt= 'b',markerfmt= '.',basefmt= 'b-')
    plt.xlabel('samples')
    plt.ylabel('bits in integer')
    plt.title('transmitted bits')
    plt.savefig('transmitted_bits.png')
    plt.show()

print(np.linalg.norm(bits - received_bits))