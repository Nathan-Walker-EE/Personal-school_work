import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt
import scipy.stats as stats
import scipy.signal as signal
from scipy.stats import norm
from scipy.special import erfc

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


zeta = 1/np.sqrt(2)
Bn = 0.05
T = 1
kp = 1 
k0 = 1
k1 = compute_K1(Bn, T, zeta, kp, k0)
k2 = compute_K2(Bn, T, zeta, kp, k0)
print(k1, k2)

#PLL system
N = 150
omega = 2*np.pi/10
feedback = 0
n = np.arange(0, N, dtype=int)
error = np.zeros(N, dtype=complex)
input = np.zeros(N, dtype=complex)
output = np.zeros(N, dtype=complex)
input_arg = np.zeros(N, dtype=complex)
output_arg = np.zeros(N, dtype=complex)
output_arg[0] = 0
output[0] = 1
int_prev = 0
prev_feedback = 0
offset = np.pi/2
J = 1j
x1 = 0
x2 = 0
x3 = 0
x4 = 0


for i in range(N):
    #multiply comp expo by conj feedback portion
    #arg is 4 quadrant tangent function
    #C.2.4-6
    # input_arg[i] = J*(omega*i + offset)
    # input[i] = np.exp(input_arg[i])

    #C.2.4.8
    input_arg[i] = (omega*i + offset)
    input[i] = np.cos(input_arg[i])
    input_arg[i] = input_arg[i] * J
    theta = input[i] * -np.sin(feedback)
    
    #c.2.4
    # tan_in = input[i] * np.conj(feedback)
    # theta = np.arctan2(np.imag(tan_in), np.real(tan_in))

    #C.2.6
    # in_arg = input[i] * np.conj(feedback)
    # theta = np.imag(in_arg)

    error[i] = theta

    #theta *= k0
    x1 = theta * k2 + x2
    vn = theta * k1 + x1
    x2 = x1
    
    #feedback portion
    x4 = x3

    #for C.2.4 and C.2.6
    #feedback = np.exp(x4*J)

    #for C.2.8
    feedback = x4
    #output[i] = feedback
    x3 = omega + k0*vn + x4
    if i < N - 1:
        #for C.2.4 and C.2.6
        # output[i + 1] = feedback
        # output_arg[i+1] = J*x4

        #for C.2.8
        output_arg[i+1] = J*x4
        output[i+1] = np.cos(feedback)

plt.plot(n, np.real(error), label='phase Error')
plt.legend()
plt.show()

plt.plot(n, np.real(output), label='output of DDS')
plt.plot(n, np.real(input), label='Input Exponential')
plt.legend()
plt.show()

plt.plot(n, np.imag(input_arg), label='input argument')
plt.plot(n, np.imag(output_arg), label='output argument')
plt.legend()
plt.show()


