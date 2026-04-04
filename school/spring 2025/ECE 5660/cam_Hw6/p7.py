import numpy as np
from functions import *
from p5 import *
import matplotlib.pyplot as plt

alpha = 0.2         # excess bandwidth
N = 11              # samples per symbol
Lp = 60             # SRRC truncation length 
Ts = 1              # symbol time
T = Ts/N            # sample time
Nsym = 200         # number of symbols
bitsPerSym = 2      # QPSK

Bn = 0.02; K = 1; A = 1; zeta = 1/np.sqrt(2); 
kp = 2; k0 = 1
k1, k2 = computeK(Bn, 1, zeta, kp, k0)
#k1 = .39; k2 = .34 # what dr moon uses

LUTI = np.array([-1, -1,  1,  1])
LUTQ = np.array([-1,  1, -1,  1])

recovered_bits = []
theta_array = []; e_array = []
x_val_preCCW = []; y_val_preCCW = []
x_got = []; y_got = []
x_rotated = []; y_rotated = []

rotate_theta = np.deg2rad(130)
theta = 0.01
x1 = 0; x2 = 0; x3 = 0; x4 = 0
k2_e_summer = 0

# Transmitter Part: -----------------------------------------------------------
# Generate random bits
bits = generateSymbols(Nsym, bitsPerSym)
encoded_bits = differential_encoder(bits)

# Map the bits onto a QPSK constellation
I, Q = mapBits(encoded_bits, bitsPerSym, LUTI, LUTQ)

# Upsample by N                 # N is the number of samples per symbol
upsampledI = upsample(I, N, Nsym)
upsampledQ = upsample(Q, N, Nsym)

# Pass through the SRRC transmitter pulse shaping filter on I and Q branches
I_t = pulseShapeFilter(upsampledI, N, Nsym, alpha, Lp, Ts)
Q_t = pulseShapeFilter(upsampledQ, N, Nsym, alpha, Lp, Ts)

# (If you were doing modulation, you would multiply by cos(Omega n)
# and -sin(Omega n) here, but we are just producing the baseband signal)

# This will produce the sequence samples of the complex baseband signal


# Channel part: ---------------------------------------------------------------
# simulate the effect of phase delay by rotating the signal by a phase
# angle of theta
# That is, multiply the complex samples by exp(j theta)
#I_t_channel, Q_t_channel = rotateSignal(I_t, Q_t, rotate_theta)
#I_t_test = np.real((I + 1j * Q)*np.exp(1j * rotate_theta))  
I_t_channel = np.real((I_t + 1j * Q_t)*np.exp(1j * rotate_theta))  
#Q_t_test = np.imag((I + 1j * Q)*np.exp(1j * rotate_theta))  
Q_t_channel = np.imag((I_t + 1j * Q_t)*np.exp(1j * rotate_theta))  


# Receiver processing ---------------------------------------------------------
cnt = N - 4
#cnt = N - 3 # for shorter range
# Matched filter on I/Q branches
x = matchedFilter(I_t_channel, N, Nsym, alpha, Lp, Ts)
y = matchedFilter(Q_t_channel, N, Nsym, alpha, Lp, Ts)

#for each sample index n:
for n in range(Nsym*N):
#for n in range(Lp*2 - N, len(x) - Lp*2): # cut off weirdos
    # (If you had multiplied by cos and -sin at the transmitter, this
    # is where you would baseband the signal by mixing with the
    # free-running oscillator)

    # Downsample
    cnt = cnt-1
    if(cnt == 0): # if this is a symbol time
        cnt = N                      
        x_val_preCCW.append(x[n]); y_val_preCCW.append(y[n])

        # compute the rotation parameters
        C = np.cos(theta)
        S = -np.sin(theta)
        # Perform the rotation
        xr = C*x[n] - S*y[n]
        yr = S*x[n] + C*y[n]
        x_rotated.append(xr); y_rotated.append(yr) # get the rotated points

        # Slice the symbols
        a_hat0 = np.sign(xr)
        a_hat1 = np.sign(yr)
        #a_hat0 = 1 if xr >= 0 else -1
        #a_hat1 = 1 if yr >= 0 else -1
        x_got.append(a_hat0)
        y_got.append(a_hat1)

        # Determine the bits
        new_bits = symbol_to_bits(a_hat0, a_hat1)
        recovered_bits.append(new_bits[0]); recovered_bits.append(new_bits[1])

        # Evaluate the ML Phase Error Detector (PED)
        e = yr * a_hat0 - xr * a_hat1
        e_array.append(e)   # save error

        # Apply the loop filter
        x1 = e * k2 + x2   
        v = e * k1 + x1     
        x2 = x1               

        # DDS (baseband)
        x4 = x3            
        x3 = v * k0 + x4   
        theta = x3     
        theta_array.append(np.rad2deg(theta)) # save theta value 

decoded_bits = differential_decoder(recovered_bits)



print(encoded_bits)
print(recovered_bits)
print(" ")
print(bits)
print(decoded_bits)
print(" ")
for a, b in zip(bits[:-10], decoded_bits[10:]):
    print(a, b)

# theta
plt.figure(1)
plt.title("theta (in degrees)")
plt.plot(range(0, len(theta_array)), theta_array)
plt.grid(True)
plt.show(block=False)

# error
plt.figure(2)
plt.title("error")
plt.plot(range(0, len(e_array)), e_array)
plt.grid(True)
plt.show(block=False)

# x and y, downsampled, but before ccw rotation
plt.figure(3)
plt.title("x,y pre ccw rotation, after downsampled")
plt.scatter(x_val_preCCW, y_val_preCCW)
plt.grid(True)
plt.show(block=False)

# x and y after ccw rotation
plt.figure(4)
plt.title("x,y after ccw rotation")
plt.scatter(x_rotated,y_rotated)
plt.grid(True)
plt.show(block=False)


print(len(bits) == len(decoded_bits))

plot_multiple_bitstreams([bits[:-10], decoded_bits[10:]], ["original bits", "decoded bits"])
plot_multiple_bitstreams([bits, decoded_bits], ["original bits", "decoded bits"])

print("K values")
print("K0 = ", k0, " and K1 = ", k1)