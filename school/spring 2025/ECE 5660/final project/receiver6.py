import numpy as np
import matplotlib.pyplot as plt
import functions as myfunc
import pulses as ps
import math
import funky as funk

problem = 6
save = False

alpha = 0.7         # excess bandwidth
N = 4               # samples per symbol
Lp = 12             # SRRC truncation length 
Ts = 1              # symbol time
T = Ts/N            # sample time

# get loop filter coefficients
Bn = 0.02/T; zeta = 1/np.sqrt(2); kp = 2; k0 = -1
k1_sync, k2_sync = myfunc.computeK(Bn, 1, zeta, kp, k0)
Bn = 0.02/T; zeta = 1; kp = 2; k0 = 1
k1_phase, k2_phase = myfunc.computeK(Bn, 1, zeta, kp, k0)
print(k1_phase, k2_phase)

M = 256
Mroot = math.floor(2**(8/2))
a = np.reshape(np.arange(-Mroot+1,Mroot,2),(2*8,1))
b = np.ones((Mroot,1))
LUT = np.hstack((np.kron(a,b), np.kron(b,a)))
Enorm = np.sum(LUT ** 2) / (2**8)
LUT = LUT/math.sqrt(Enorm)
UW = [162,29,92,47,16,112,63,234,50,7,15,211,109,124,239,
      255,243,134,119,40,134,158,182,0,101,62,176,152,228,36]

# get sync pulse
srrc = ps.srrc_pulse(N, Lp, Ts, alpha)

# get signal
r = myfunc.readInputFile("sim"+str(problem)+"_2025")
r = np.array(r)

# demodulate
Omega0 = np.pi/2
n = np.arange(r.size)
cosine = np.sqrt(2)*np.cos(Omega0*n)
sine = np.sqrt(2)*np.sin(Omega0*n)

I = r * cosine
Q = r * sine * (-1)

# Receiver Part: --------------------------------------------------------------
L = 5
beta = 6
window = np.kaiser(2*L + 1, beta)

diffCoeffs, delayCoeffs = funk.build_differentiator(L, window, Ts)

pulse_delayed = np.convolve(srrc, delayCoeffs)
pulse_diffed = np.convolve(srrc, diffCoeffs)

# do the matched filter
x = np.convolve(I, delayCoeffs)
x = np.convolve(x, srrc)
y = np.convolve(Q, delayCoeffs)
y = np.convolve(y, srrc)

# derivative matched filter
dx = np.convolve(I, pulse_diffed)
dy = np.convolve(Q, pulse_diffed)

# initial values
NCO1 = 0.0; W1 = 0.0; MU1 = 0.0; V21 = 0.0; e_sync = 0

theta = 0.0; x1 = 0; x2 = 0; x3 = 0; x4 = 0
LUT_complex = LUT[:, 0] + 1j * LUT[:, 1]

# arrays
mu_array = []; theta_array = []; x_rot = []; y_rot = []
a0 = []; a1 = []; e_sync_array = []; e_phase_array = []
a0_no_int = []; a1_no_int = []

# the loop
for n in range(len(x)):
    # compute NCO, strobe, mu
    temp = NCO1 - W1

    if temp < 0:
        strobe = 1
        mu = NCO1/W1
        nco = 1 + temp
        mu_array.append(mu)
    else:
        strobe = 0
        mu = MU1
        nco = temp

    if strobe == 0:
        e_sync = 0
    else:
        if n > 3 and n < len(x) - 3:
            # do the interpolation
            xi = funk.farrow_cubic_interp(x[n-3:n+1], mu) 
            dxi = funk.farrow_cubic_interp(dx[n-3:n+1], mu) 
            yi = funk.farrow_cubic_interp(y[n-3:n+1], mu) 
            dyi = funk.farrow_cubic_interp(dy[n-3:n+1], mu) 

            # compute the rotation parameters
            C = np.cos(theta)
            S = -np.sin(theta)

            # Perform the rotation
            xr = C*xi - S*yi
            yr = S*xi + C*yi
            dxr = C*dxi - S*dyi
            dyr = S*dxi + C*dyi
            x_rot.append(xr); y_rot.append(yr) # save the rotated points

            # slice symbols
            # [a_hat0, a_hat1, point] = funk.mapToConstellation(xr, yr, LUT)
            corrected = xr + 1j*yr
            decision = funk.slicer_256qam(corrected, LUT_complex)
            a_hat0 = np.real(decision)
            a_hat1 = np.imag(decision)
            # a_hat0 = a_hat0[0]; a_hat1 = a_hat1[0]

            # compute e
            e_sync = (a_hat0*dxr) + (a_hat1*dyr)
            e_sync_array.append(e_sync)

            e_phase = yr * a_hat0 - xr * a_hat1
            e_phase_array.append(e_phase)   # save error

            # Apply the loop filter
            x1 = e_phase * k2_phase + x2   
            v_phase = e_phase * k1_phase + x1     
            x2 = x1               

            # DDS (baseband)
            x4 = x3            
            x3 = v_phase * k0 + x4   
            theta = x3    
            theta_array.append(theta)

            # const points
            a0_no_int.append(x[n]/N)
            a1_no_int.append(y[n]/N)
            a0.append(xr)
            a1.append(yr)

    # compute loop filter
    v1 = k1_sync * e_sync * 0.99
    v2 = V21 + k2_sync * e_sync
    v_sync = v1 + v2
    V21 = v2 * 0.99

    # compute NCO input
    w = v_sync + (1/N)
    W1 = w
    NCO1 = nco
    MU1 = mu

funk.plot_scatter(x_rot, y_rot)
funk.plot_error_signal(e_sync_array, 'Timing Error')
funk.plot_error_signal(e_phase_array, 'Phase Error')
funk.plot_fractional_interval(mu_array)
funk.plot_phase_signal(theta_array)

x_rot = np.array(x_rot)
y_rot = np.array(y_rot)
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



