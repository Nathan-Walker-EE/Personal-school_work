import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal

def build_differentiator(L, window, T):
    diff = np.zeros(2*L + 1)
    for i in range(len(diff)):
        if i - L != 0:
            diff[i] = (-1)**(i-L)/(i-L)
        else:
            diff[i] = 0
    diff = diff * window/T
    delay = np.zeros(2*L + 1)
    delay[L] = 1
    return diff, delay

L = 5
window = np.ones(2*L + 1)
T = 1
N = 1024
beta = 6
window = np.kaiser(2*L + 1, beta)

[diff, delay] = build_differentiator(L, window, T)
N = 1024
D = np.fft.fftshift(np.fft.fft(diff, N))  # FFT of differentiator
D_delay = np.fft.fftshift(np.fft.fft(delay, N))  # FFT of delay filter
n = np.arange(0, N) / N  # Normalize frequency axis
n = n - 0.5  # Shift to range [-0.5, 0.5]

# 1. Differentiator Impulse Response
plt.figure(figsize=(6, 4))
plt.stem(diff, basefmt="k-")
plt.xlabel("Sample Index")
plt.ylabel("Amplitude")
plt.title("Impulse Response of Differentiator")
plt.grid(True)
plt.show()

# 2. Delay Filter Impulse Response
plt.figure(figsize=(6, 4))
plt.stem(delay, basefmt="k-")
plt.xlabel("Sample Index")
plt.ylabel("Amplitude")
plt.title("Impulse Response of Delay Filter")
plt.grid(True)
plt.show()

# 3. Magnitude Response of Differentiator
plt.figure(figsize=(8, 5))
plt.plot(n, np.abs(D), label="Magnitude Response", color='b')
plt.axvline(x=-0.3, color='r', linestyle='dashed', label="Cutoff (-0.3)")
plt.axvline(x=0.3, color='r', linestyle='dashed', label="Cutoff (0.3)")

plt.figure(figsize=(8, 5))
plt.plot(n, np.abs(D_delay), label="Magnitude Response", color='g')
plt.axvline(x=-0.3, color='r', linestyle='dashed', label="Cutoff (-0.3)")
plt.axvline(x=0.3, color='r', linestyle='dashed', label="Cutoff (0.3)")

# Labels and title
plt.xlabel("Normalized Frequency")
plt.ylabel("Magnitude")
plt.title("Magnitude Response of Delay Filter")
plt.legend()
plt.grid(True)

plt.show()

# Labels and title
plt.xlabel("Normalized Frequency")
plt.ylabel("Magnitude")
plt.title("Magnitude Response of Differentiator")
plt.legend()
plt.grid(True)

plt.show()

t = np.arange(0,150)
A = 1
f = .05
w = f * 2 * np.pi
ft = A * np.sin(w*t)
ftp = np.convolve(ft, diff)
ftd = np.convolve(ft, delay)
plt.figure(figsize=(8, 5))  # Set figure size
plt.plot(ftp, label="sin after differentiation", color='b', linewidth=2)  # Blue line
plt.plot(ftd, label="sin after delay", color='r', linestyle='dashed', linewidth=2)  # Red dashed line

# Labels and title
plt.xlabel("samples")
plt.ylabel("amplitude")
plt.title("Comparisons of filter outputs")

# Grid, legend, and layout
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)  # Dashed grid lines for readability
plt.show()
