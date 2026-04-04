import numpy as np
import math
import matplotlib.pyplot as plt
import funky as funk

Ts = 1                              # sample period
N = 4                               # samples per symbol
alpha = 0.7                         # excess bandwidth
Lp = 12                             # pulse length
srrc = funk.srrc1(alpha, N, Lp)    # pulse
w0 = math.pi/2                      # carrier frequency

# Loop filter coefficients (tune as needed)
zeta = 1/np.sqrt(2)
Bn = 0.05
T = 1
kp = 1
k0 = 1

k1 = funk.compute_K1(Bn, T, zeta, kp, k0)
k2 = funk.compute_K2(Bn, T, zeta, kp, k0)
print(k1, k2)

LUT, Enorm = funk.getLUT()

filename = 'sim3_2025'
r = funk.readInputFile(filename)
r = np.array(r)

n = np.arange(r.size)
cosine = math.sqrt(2)*np.cos(w0*n)
sine = math.sqrt(2)*np.sin(w0*n)
I = r * cosine
Q = r * sine * (-1)

I_x = np.convolve(I, srrc, mode= 'same')
Q_x = np.convolve(Q, srrc, mode= 'same')

x = []
y = []
x1 = 0
x2 = 0
e_buf = []
phase_buf = []
phase = 0.0
LUT_complex = LUT[:, 0] + 1j * LUT[:, 1]
output = []
cnt = N 
for n in range(Lp*4 - (N - 2), len(I_x) - Lp*4):
    cnt -= 1
    if cnt == 0:
        cnt = N
        symbol = I_x[n] + 1j * Q_x[n]
        corrected = symbol * np.exp(-1j * phase)
        phase_buf.append(phase)
        decision = funk.slicer_256qam(corrected, LUT_complex)
        error = np.imag(corrected * np.conj(decision))
        x1 = error * k2 + x2
        v = error * k1 + x1
        x2 = x1
        phase += v
        output.append(corrected)
        e_buf.append(error)
        

plt.plot(e_buf)
plt.show()
output = np.array(output)
x_corrected = np.real(output)
y_corrected = np.imag(output)
funk.plot_scatter(x_corrected,y_corrected)
funk.plot_error_signal(e_buf, 'Phase Error')
funk.plot_phase_signal(phase_buf)

phases = [0, 90, 180, 270]
for i in range(4):
    phase_set = np.exp(1j * np.deg2rad(phases[i]))
    S = output * phase_set
    tempx = np.real(S)
    tempy = np.imag(S)
    constx, consty, points = funk.mapToConstellation(tempx, tempy, LUT)
    funk.drawKitties(points.tolist(), 130, filename)
    plt.show()
