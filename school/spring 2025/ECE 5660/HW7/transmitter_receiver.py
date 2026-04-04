import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt
import pulses as ps
import differentiator as df
import Interpolator as interp

def symbol_to_bits(I, Q):
    if I < 0 and Q < 0:
        return [1, 0]
    if I < 0 and Q > 0:
        return [1, 1]
    if I > 0  and Q < 0:
        return [0, 1]
    if I > 0 and Q > 0:
        return [0, 0]
    if I == 0 and Q == 0:
        return [0, 0]

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
    for i in range(0, len(bits), 2):
        bk = bits[i]
        bk1 = bits[i+1]
        [Idata, Qdata] = constellation_map[(bk, bk1)]
        I.append(Idata)
        Q.append(Qdata)

    return [np.array(I), np.array(Q)]

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

zeta = 1/np.sqrt(2); Bn = 0.02; T = 1; kp = 2 ; k0 = 1
k1 = compute_K1(Bn, T, zeta, kp, k0)
k2 = compute_K2(Bn, T, zeta, kp, k0)
k1 = -0.028886
k2 = -0.000096286
cubic = 1
linear = 0
a = 1
alpha = 1 # excess bandwidth
N = 4 # samples per symbol
Lp = 60 # SRRC truncation length
Ts = 1 # symbol time
T = Ts/N # sample time
L = 5
beta = 6
window = np.kaiser(2*L + 1, beta)
[diff, delay] = df.build_differentiator(L, window, Ts)

pulse = ps.srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse
mf_dif = np.convolve(pulse, diff)
mf_delay = np.convolve(pulse, delay)

Nsym = 1000
bitstream = np.random.randint(0,2,2*Nsym)
[I,Q] = map_to_QPSK(bitstream)
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
S_complex = sI + sQ*1j
sIp = np.real(S_complex)
sQp = np.imag(S_complex)

## start of receiver
#derivative and delay filter
xdelay = np.convolve(mf_delay, sIp)
ydelay = np.convolve(mf_delay, sQp)
xdiff  = np.convolve(mf_dif, sIp)
ydiff  = np.convolve(mf_dif, sQp)

x_buf = []
y_buf = []
mu_buf = []
err_buf = []
W1 = 0
NCO1 = 0
MU1 = 0
v21 = 0.0

for i in range(120-N,len(xdelay)-120):
    #MATLAB to python from p493
    temp = NCO1 - W1
    if temp < 0:
        strobe = 1
        mu = NCO1/W1
        mu_buf.append(mu)
        nco = 1 + temp
    else:
        strobe = 0
        mu = MU1
        nco = temp

    if strobe == 0:
        error = 0
    else:
        if linear == 1:
            FX = xdelay[i:i+2]
            FXD = xdiff[i:i+2]
            xi = interp.farrow_linear_interp(FX, mu)
            xid = interp.farrow_linear_interp(FXD, mu)
            FY = ydelay[i:i+2]
            FYD = ydiff[i:i+2]
            yi = interp.farrow_linear_interp(FY, mu)
            yid = interp.farrow_linear_interp(FYD, mu)
        if cubic == 1:
            FX = xdelay[i-1:i+3]
            FXD = xdiff[i-1:i+3]
            xi = interp.farrow_cubic_interp(FX, mu)
            xid = interp.farrow_cubic_interp(FXD, mu)
            FY = ydelay[i-1:i+3]
            FYD = ydiff[i-1:i+3]
            yi = interp.farrow_cubic_interp(FY, mu)
            yid = interp.farrow_cubic_interp(FYD, mu)

        x_buf.append(xi)
        y_buf.append(yi)
        
        new_bits = symbol_to_bits(xi, yi)
        error = (np.sign(xi)*xid) + (np.sign(yi)*yid)
        err_buf.append(error)

    v1 = k1 * error * .99
    v2 = v21 + k2 * error
    vn = v1 + v2

    wn = 1/N + vn
    v21 = 0.99 * v2
    NCO1 = nco
    MU1 = mu
    W1 = wn

# Plot error buffer
plt.figure(figsize=(6, 4))  # Set figure size
plt.plot(err_buf, color='b', label="Error Signal")
plt.xlabel("Iteration")  # X-axis label
plt.ylabel("Error Value")  # Y-axis label
plt.title("Error Signal Over Time")  # Title
plt.legend()  # Add legend
plt.grid(True)  # Add grid
plt.show()

# Plot mu buffer (Timing Error)
plt.figure(figsize=(6, 4))
plt.plot(mu_buf, color='r', label="Timing Error (mu)")
plt.xlabel("Iteration")
plt.ylabel("Mu Value")
plt.title("Timing Error Convergence")
plt.legend()
plt.grid(True)
plt.show()

# Scatter plot of x_buf vs y_buf
plt.figure(figsize=(6, 4))
plt.scatter(x_buf, y_buf, color='g', alpha=0.6, edgecolors='black', label="Received Symbols")
plt.xlabel("In-phase Component (I)")
plt.ylabel("Quadrature Component (Q)")
plt.title("Received Signal Constellation")
plt.legend()
plt.grid(True)
plt.show()

x_buff = np.sign(x_buf)
y_buff = np.sign(y_buf)
rbits = []
for i in range(len(x_buff)):
    rbits.append(symbol_to_bits(x_buff[i], y_buff[i]))

rbits_f = np.array(rbits).flatten()
# 4 if N = 4 and cubic interp
# 2 if N = 16 and linear interp
rbits_f = rbits_f[4:len(rbits_f)-4]

for i in range(len(bitstream)):
    if rbits_f[i] != bitstream[i]:  # Compare elements
        a = 1
        print(f"Mismatch at index {i}: {rbits_f[i]} != {bitstream[i]}")

fig, ax = plt.subplots(2, 1, figsize=(8, 6))

# First subplot
ax[0].plot(bitstream, label="input bits", color='b')
ax[0].set_title("Input bits")
ax[0].legend()
ax[0].grid()

# Second subplot
ax[1].plot(rbits_f, label="output bits", color='b')
ax[1].set_title("Output bits")
ax[1].legend()
ax[1].grid()

# Show the figure
plt.tight_layout()  # Adjusts spacing
plt.show()

N = 1024
D = np.fft.fftshift(np.fft.fft(diff, N))
n = np.arange(0, N) / N  # Normalize frequency axis
n = n - 0.5  # Shift to range [-0.5, 0.5]

plt.figure(figsize=(8, 5))
plt.plot(n, np.abs(D), label="Magnitude Spectrum", color='b')
plt.axvline(x=-0.3, color='r', linestyle='dashed', label="Cutoff (-0.3)")
plt.axvline(x=0.3, color='r', linestyle='dashed', label="Cutoff (0.3)")

# Labels and title
plt.xlabel("Normalized Frequency")
plt.ylabel("Magnitude")
plt.title("Magnitude Spectrum of Differentiator")
plt.legend()
plt.grid(True)

plt.show()

plt.figure(figsize=(8, 5))
plt.plot(n, np.angle(D), label="Phase Response", color='b')
plt.axvline(x=-0.3, color='r', linestyle='dashed', label="Cutoff (-0.3)")
plt.axvline(x=0.3, color='r', linestyle='dashed', label="Cutoff (0.3)")

# Labels and title
plt.xlabel("Normalized Frequency")
plt.ylabel("Phase (radians)")
plt.title("Phase Response of Differentiator")
plt.legend()
plt.grid(True)

plt.show()