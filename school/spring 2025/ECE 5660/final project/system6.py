import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt
import pulses as ps
import funky as funk

zeta = 1/np.sqrt(2); Bn = 0.05; T = 1; kp = 1 ; k0 = 1
pk1 = funk.compute_K1(Bn, T, zeta, kp, k0)
pk2 = funk.compute_K2(Bn, T, zeta, kp, k0)
zeta = 1/np.sqrt(2); Bn = 0.0145; T = 1; kp = 2 ; k0 = 1
tk1 = funk.compute_K1(Bn, T, zeta, kp, k0)
tk2 = funk.compute_K2(Bn, T, zeta, kp, k0)
a = 1
alpha = .7 # excess bandwidth
N = 4 # samples per symbol
Lp = 12 # SRRC truncation length
Ts = 1 # symbol time
T = Ts/N # sample time
pulse = ps.srrc_pulse(N, Lp, Ts, alpha); # get the square-root raised cosine pulse
L = 5
beta = 6
window = np.kaiser(2*L + 1, beta)
[diff, delay] = funk.build_differentiator(L,window,Ts)
mf_dif = np.convolve(pulse, diff)
mf_delay = np.convolve(pulse, delay)

M = 256
Mroot = math.floor(2**(8/2))
a = np.reshape(np.arange(-Mroot+1,Mroot,2),(2*8,1))
b = np.ones((Mroot,1))
LUT = np.hstack((np.kron(a,b), np.kron(b,a)))
Enorm = np.sum(LUT ** 2) / (2**8)
LUT = LUT/math.sqrt(Enorm)

## start of receiver
filename = "sim6_2025"
r = funk.readInputFile(filename)
print(len(r))
r = np.array(r)
n = np.arange(r.size)
cosine = math.sqrt(2)*np.cos(math.pi*n/2)
sine = math.sqrt(2)*np.sin(math.pi*n/2)

I = r * cosine
Q = -r * sine

# matched filter
Id = np.convolve(mf_dif, I)
If = np.convolve(mf_delay, I)
Qd = np.convolve(mf_dif, Q)
Qf = np.convolve(mf_delay, Q)
x = []
y = []

cnt = N
LUT_complex = LUT[:, 0] + 1j * LUT[:, 1]
bits_received = []
x1 = 0
x2 = 0
theta = 0.0
W1 = 0
NCO1 = 0.0
MU1 = 0
v21 = 0.0
perror = []
terror = []
mu_buf = []
ebuf  = []
theta_buf = []

for i in range(Lp*4 - (N-2), len(If) - 4 * Lp):
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
        FX = If[i-1:i+3]
        FXD = Id[i-1:i+3]
        xi = funk.farrow_cubic_interp(FX, mu)
        xid = funk.farrow_cubic_interp(FXD, mu)

        FY = Qf[i-1:i+3]
        FYD = Qd[i-1:i+3]
        yi = funk.farrow_cubic_interp(FY, mu)
        yid = funk.farrow_cubic_interp(FYD, mu)

        dif_symbol = xid + 1j*yid
        symbol = xi + 1j*yid

        dif_corrected = dif_symbol*np.exp(1j*theta)
        corrected = symbol*np.exp(1j*theta)
        theta_buf.append(theta)
        x.append(np.real(corrected))
        y.append(np.imag(corrected))

        [ao, a1, index] = funk.mapToConstellation(xi, yi, LUT)
        error = ao * np.real(dif_corrected) + a1*np.imag(dif_corrected)
        ebuf.append(error)

        decision = funk.slicer_256qam(corrected, LUT_complex)
        phase_error = np.imag(corrected * np.conj(decision))
        perror.append(phase_error)
        x1 = phase_error * pk2 + x2
        v = phase_error * pk1 + x1
        x2 = x1
        theta += v
    
    v1 = tk1 * error * .99
    v2 = v21 + tk2 *error
    vn = v1 + v2

    wn = 1/N + vn
    v21 = 0.99 * v2
    NCO1 = nco
    MU1 = mu
    W1 = wn

funk.plot_scatter(x, y)
funk.plot_error_signal(ebuf, 'Timing Error')
funk.plot_error_signal(perror, 'Phase Error')
funk.plot_fractional_interval(mu_buf)
funk.plot_phase_signal(theta_buf)

x_rot = np.array(x)
y_rot = np.array(y)
output = x_rot + 1j*y_rot
phases = [0, 90, 180, 270]
for i in range(4):
    phase_set = np.exp(1j * np.deg2rad(phases[i]))
    S = output * phase_set
    tempx = np.real(S)
    tempy = np.imag(S)
    constx, consty, points = funk.mapToConstellation(tempx, tempy, LUT)
    funk.drawKitties(points.tolist(), 130, 'sim6')
    plt.show()
