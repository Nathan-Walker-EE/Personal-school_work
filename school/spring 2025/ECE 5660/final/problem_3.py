import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import welch

# part a
output = 0
x0 = 1
x1 = 0
x2 = 0
x3 = 0
x4 = 0
output_buf = []
for i in range(35):
    print(i, x0, x1, x2, x3, x4)
    output = x4
    output_buf.append(output)
    x4 = x3
    x3 = x2
    x2 = (x1 + output)%2
    x1 = x0
    x0 = output

output_buf = np.array(output_buf)
c = (-1)**output_buf
print(c)
Tc = 1
T = 35 * Tc
t = np.linspace(0, Tc, 1000)
pt = 1/np.sqrt(Tc) *((t >= 0) & (t < Tc))
# plt.plot(pt)
c_up = []
for i in range(len(c)):
    temp = np.zeros(1000)
    temp[0] = c[i]
    c_up.append(temp)
c_up = np.array(c_up)
c_t = np.convolve(c_up.flatten(), pt)
c_t = c_t[0 : 35000]
# figure out a way to plot c

x = np.linspace(0, T, 1000*35)
plt.figure(figsize=(10, 3))
plt.plot(x, c_t, label='c(t)')
plt.title('Spread Spectrum Signal c(t)')
plt.xlabel('Time')
plt.ylabel('Amplitude')
plt.grid(True)
plt.tight_layout()
plt.show()

# part b
Fs = 1
C = np.fft.fftshift(np.fft.fft(c_t))
PSD = np.abs(C)**2
plt.figure(figsize=(8, 3))
n = np.arange(0, len(C))/len(C)
n = n - .5
plt.plot(n,PSD)
plt.title('Power Spectral Density of c(t)')
plt.xlabel('bin')
plt.ylabel('Magnitude')
plt.grid(True)
plt.tight_layout()
plt.show()

# part c
Ts = 5*Tc
bits = np.array([0, 1, 0, 1, 1, 0, 1])
symbols = (-1)**bits
t = np.linspace(0, Ts, 1000*5)
pt = 1/np.sqrt(Ts) *((t >= 0) & (t < Ts))
# plt.plot(pt)
s_up = []
for i in range(len(symbols)):
    temp = np.zeros(1000*5)
    temp[0] = symbols[i]
    s_up.append(temp)
s_up = np.array(s_up)
s = np.convolve(s_up.flatten(), pt)
s = s[0:35000]
plt.plot(s)
plt.show()

# part d 
Fs = 1
S = np.fft.fftshift(np.fft.fft(s))
SPSD = np.abs(S)**2
plt.figure(figsize=(8, 3))
n = np.arange(0, len(S))/len(S)
n = n - .5
plt.plot(n,SPSD)
plt.title('Power Spectral Density of s(t)')
plt.xlabel('bin')
plt.ylabel('Magnitude')
plt.grid(True)
plt.tight_layout()
plt.show()

# part e 
x = s * c_t
plt.plot(x)
plt.show()

# part f 
Fs = 1
X = np.fft.fftshift(np.fft.fft(x))
XPSD = np.abs(X)**2
plt.figure(figsize=(8, 3))
n = np.arange(0, len(X))/len(X)
n = n - .5
plt.plot(n,XPSD)
plt.title('Power Spectral Density of x(t)')
plt.xlabel('bin')
plt.ylabel('Magnitude')
plt.grid(True)
plt.tight_layout()
plt.show()
