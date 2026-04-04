import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt
import pulses as ps

def symbol_to_bits(I, Q):
    if I == -1 and Q == -1:
        return [1, 0]
    if I == -1 and Q == 1:
        return [1, 1]
    if I == 1 and Q == -1:
        return [0, 1]
    if I == 1 and Q == 1:
        return [0, 0]

def encode_differential_qpsk2(bitstream):
    LUT = (
        (0, 0), (0, 1), (1, 0), (1, 1),
        (1, 0), (0, 0), (1, 1), (0, 1),
        (0, 1), (1, 1), (0, 0), (1, 0),
        (1, 1), (1, 0), (0, 1), (0, 0)
    )
    delay_bits = (0, 0)
    out_bits = []

    for i in range(0, len(bitstream), 2):
        bits = bitstream[i:i+2]
        LUT_index = bits[0] * 8 + bits[1] * 4 + delay_bits[0] * 2 + delay_bits[1]
        delay_bits = LUT[LUT_index]
        
        out_bits.append(delay_bits[0]); out_bits.append(delay_bits[1])
    return out_bits

def decode_differential_qpsk2(bitstream):
    LUT = (
        (0, 0), (1, 0), (0, 1), (1, 1),
        (0, 1), (0, 0), (1, 1), (1, 0),
        (1, 0), (1, 1), (0, 0), (0, 1),
        (1, 1), (0, 1), (1, 0), (0, 0)
    )
    delay_bits = (0, 0)
    out_bits = []

    for i in range(0, len(bitstream), 2):
        bits = bitstream[i:i+2]
        LUT_index = delay_bits[0] * 8 + delay_bits[1] * 4 + bits[0] * 2 + bits[1]
        decoded_bits = LUT[LUT_index]

        out_bits.append(decoded_bits[0]); out_bits.append(decoded_bits[1])

        delay_bits = bits
    return out_bits

def map_to_QPSK(bits):
    constellation_map = {
        (0, 0) : (1, 1),
        (0, 1) : (1, -1),
        (1, 0) : (-1, -1),
        (1, 1) : (-1, 1)
    }
    bk = 0
    bk1 = 1
    I = []
    Q = []
    #print("\nmappin")
    for i in range(0, len(bits), 2):
        bk = bits[i]
        bk1 = bits[i+1]
        [Idata, Qdata] = constellation_map[(bk, bk1)]
        I.append(Idata)
        Q.append(Qdata)
        #print(bk, bk1, "   ", Idata, Qdata)
    #print("\n\n")

    return [np.array(I), np.array(Q)]

def QPSK_decision(I, Q):
    out_bits = []
    for i in range(0,len(I), 1):
        if I[i] > 0:
            if Q[i] > 0:
                a0 = 0
                a1 = 0
            else:
                a0 = 0
                a1 = 1
        else:
            if Q[i] > 0:
                a0 = 1
                a1 = 1
            else:
                a0 = 1
                a1 = 0
        out_bits.append(a0)
        out_bits.append(a1)
    
    return out_bits

def compute_K1(Bn, T, zeta, kp, k0):
    num = (4*zeta * Bn * T)/(zeta + 1/(4*zeta))
    den = 1 + (2 * zeta*Bn*T)/(zeta + 1/(4*zeta)) + ((Bn*T)/(zeta + 1/(4*zeta)))**2
    k1 = num/den/kp/k0
    return k1

def compute_K2(Bn, T, zeta, kp, k0):
    num = 4*((Bn * T)/(zeta + 1/(4*zeta)))**2
    den = 1 + (2 * zeta*Bn*T)/(zeta + 1/(4*zeta)) + ((Bn*T)/(zeta + 1/(4*zeta)))**2
    k2 = num/den/kp/k0
    return k2

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

zeta = 1/np.sqrt(2); Bn = 0.02; T = 1; kp = 2 ; k0 = 1
k1 = compute_K1(Bn, T, zeta, kp, k0)
k2 = compute_K2(Bn, T, zeta, kp, k0)
x1 = 0
x2 = 0
x3 = 0
x4 = 0
a = 1
J = 1j 
alpha = .2 # excess bandwidth
N = 11 # samples per symbol
Lp = 60 # SRRC truncation length
Ts = 1 # symbol time
T = Ts/N # sample time
pulse = ps.srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse

Nsym = 1000
bitstream = np.random.randint(0,2,2*Nsym)
#encoded_symbols = bitstream
encoded_symbols = encode_differential_qpsk2(bitstream)
[I,Q] = map_to_QPSK(encoded_symbols)
I_upsampled = np.zeros((N*Nsym,1))
Q_upsampled = np.zeros((N*Nsym,1))
I_upsampled[range(0,N*Nsym,N)] = I.reshape(Nsym,1)
Q_upsampled[range(0,N*Nsym,N)] = Q.reshape(Nsym,1)

# Pulse shape
#convolve upsampled bitstream with pulse
I = np.convolve(I_upsampled.flatten(), pulse)
Q = np.convolve(Q_upsampled.flatten(), pulse)

#multiply by cos scaled by np.sqrt(2)
n = np.arange(0, len(I))
sI = I 
sQ = Q 

## end of transmitter

##Channel
#phase = np.exp(np.deg2rad(130*1j))
phase = np.exp(1j*np.deg2rad(40))
S_complex = sI + J*sQ
S_complex = S_complex * phase
sIp = np.real(S_complex)
sQp = np.imag(S_complex)

## start of receiver
# multiply by np.sqrt(2)
rI = sIp
rQ = sQp
# matched filter
x = np.convolve(pulse, rI)
y = np.convolve(pulse, rQ)
x = x[120:len(x)-120]
y = y[120:len(y)-120]

theta = np.deg2rad(0)
cnt = 1
j = 0
received_bits = []
received_x = []
received_y = []
error_array = []
theta_ar = []
received_symbols = []

for i in range(len(x)):
    cnt = cnt-1
    if(cnt == 0):
        cnt = N
        C = np.cos(theta)
        S = -np.sin(theta)
        theta_ar.append(theta)
        xr = C*x[i] - S*y[i]
        yr = S*x[i] + C*y[i]
        a0 = np.sign(xr)
        a1 = np.sign(yr)
        
        error = (yr * a0) - (xr * a1)
        x1 = error * k2 + x2
        theta = error * k1 + x1
        x2 = x1

        x4 = x3
        x3 = theta * k0 + x4
        theta = x3

        received_x.append(xr)
        received_y.append(yr)
        received_symbols.append(int(a0))
        received_symbols.append(int(a1))
        new_bits = symbol_to_bits(a0, a1)
        received_bits.append(new_bits[0])
        received_bits.append(new_bits[1])
        error_array.append(error)

#received_symbols = received_x + 1j * np.array(received_y)  # Convert to complex form
decoded_bits = decode_differential_qpsk2(received_bits)
#decoded_bits = received_symbols
for i in range(len(decoded_bits)):
    if decoded_bits[i] != bitstream[i]:  # Compare elements
        print(f"Mismatch at index {i}: {decoded_bits[i]} != {bitstream[i]}")
err = decoded_bits - bitstream
plt.plot(decoded_bits)
plt.plot(bitstream)
plt.show()
print(np.linalg.norm(err))

plt.scatter(received_x, received_y)
plt.show()

plt.plot(error_array)
plt.show()
plt.plot(theta_ar)
plt.show()

