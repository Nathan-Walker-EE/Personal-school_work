import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as signal

def farrow_linear_interp(FX, mu):
    VF1 = FX[0] - FX[1]
    VF2 = FX[1]
    return mu * VF1 + VF2

def farrow_cubic_interp(FX, mu):
    vF3 = FX[3]/6 + FX[2]/-2 + FX[1]/2 + FX[0]/-6
    vF2 = FX[2]/2 - FX[1] + FX[0]
    vF1 = FX[3]/-6 + FX[2] + FX[1]/-2 + FX[0]/-3
    vF0 = FX[1]
    return ((mu*vF3+vF2)*mu + vF1)*mu + vF0


F0T = .15
T = 1
t_samples = np.array([0, T, 2*T, 3*T])
y_samples = np.sin(2*np.pi*F0T * t_samples)

t_interp = np.linspace(T, 2*T, 11)
linear_interp = []
cubic_interp = []

FX0 = y_samples[3]
FX1 = y_samples[2]
FX2 = y_samples[1]
FX3 = y_samples[0]

i_range = [y_samples[2], y_samples[1]]
for t in t_interp:
    mu_i = (t-T)/T
    linear_interp.append(farrow_linear_interp(i_range,mu_i))

for t in t_interp:
    mu = (t-T)/T
    # vF3 = FX0/6 + FX1/-2 + FX2/2 + FX3/-6
    # vF2 = FX1/2 - FX2 + FX3
    # vF1 = FX0/-6 + FX1 + FX2/-2 + FX3/-3
    # vF0 = FX2
    # xi = ((mu*vF3+vF2)*mu + vF1)*mu + vF0
    # cubic_interp.append(xi)
    cubic_interp.append(farrow_cubic_interp(y_samples,mu))

true_signal = np.sin(2*np.pi*F0T*t_interp)

# plt.plot(t_interp, true_signal, 'k--', label="true sin wave")
# plt.plot(t_interp, linear_interp, 'bo-', label="linear interpolation")
# plt.plot(t_interp, cubic_interp, 'ro-', label="cubic interpolation")

# plt.xlabel("Time")
# plt.ylabel("Amplitude")
# plt.legend()
# plt.grid()
# plt.title("Farrow Interpolation: Linear vs Cubic")
# plt.show()