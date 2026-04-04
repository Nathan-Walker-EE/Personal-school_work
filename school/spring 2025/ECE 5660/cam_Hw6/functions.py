import random
import math
import numpy as np
import matplotlib.pyplot as plt

def generateSymbols(Nsym, bitsPerSym):
    bitList = [random.randint(0, 1) for _ in range(Nsym * bitsPerSym)]
    return bitList


def mapBits(bits, bitsPerSym, LUTI, LUTQ):
    bits = np.array(bits)
    symbol = bits.reshape(-1, bitsPerSym)              
    symbolVal = symbol[:, 0] * 2 + symbol[:, 1]  
    ampI = LUTI[symbolVal]
    ampQ = LUTQ[symbolVal]
    return ampI, ampQ


def upsample(sym, N, Nsym): 
    upsampled = np.zeros((N*Nsym,1))
    upsampled[range(0,N*Nsym,N)] = sym.reshape(Nsym, 1)
    return upsampled


def srrc1(alpha, N, Lp, Ts):
    srrc_samples = np.zeros(2 * Lp)

    for n in range(-Lp, Lp): 
        if n == 0: # handle n = 0 divide by 0
            sample = (1 + alpha * (4 / math.pi - 1))
            sample = 1 / math.sqrt(N) * sample 
        elif abs(n) == N / (4 * alpha): # handle n = N/4alpha divide by 0
            sample = ((1 + 2 / math.pi) * math.sin(math.pi / (4 * alpha)) +
                      (1 - 2 / math.pi) * math.cos(math.pi / (4 * alpha)))
            sample = alpha / math.sqrt(N) * sample
        else:
            num = math.sin(math.pi * (1 - alpha) * n / N)
            num += (4 * alpha * n) / N * math.cos(math.pi * (1 + alpha) * n / N)
            denom = 4 * alpha * n / N
            denom = 1 - denom * denom
            denom = denom * math.pi * n / N
            sample = (1 / math.sqrt(N)) * num / denom
        
        srrc_samples[n + Lp] = sample

    return srrc_samples 


def pulseShapeFilter(upsampled, N, Nsym, alpha, Lp, Ts):
    pulse_shape = srrc1(alpha, N, Lp, Ts)
    s = np.convolve(upsampled.reshape((N*Nsym,)),pulse_shape, mode='same')
    return s


def rotateSignal(I, Q, theta):
    theta = np.deg2rad(theta)  # Convert degrees to radians
    phase_shift = np.exp(1j * theta)  # Compute complex phase shift
    
    complex_signal = I + 1j * Q  
    rotated_signal = complex_signal * phase_shift

    return np.real(rotated_signal), np.imag(rotated_signal)


def matchedFilter(s, N, Nsym, alpha, Lp, Ts):
    pulse_shape = srrc1(alpha, N, Lp, Ts)
    s = np.convolve(s, pulse_shape)
    return s


def symbol_to_bits(I, Q):
    if I == -1 and Q == -1:
        return [0, 0]
    if I == -1 and Q == 1:
        return [0, 1]
    if I == 1 and Q == -1:
        return [1, 0]
    if I == 1 and Q == 1:
        return [1, 1]
    

def computeK(Bn, T, zeta, kp, k0):
    num1 = (4*zeta * Bn * T)/(zeta + 1/(4*zeta))
    num2 = 4*((Bn * T)/(zeta + 1/(4*zeta)))**2
    den = 1 + (2 * zeta*Bn*T)/(zeta + 1/(4*zeta)) + ((Bn*T)/(zeta + 1/(4*zeta)))**2
    k1 = num1/den/kp/k0
    k2 = num2/den/kp/k0
    return k1, k2














def plot_multiple_bitstreams(bitstreams, labels):
    plt.figure(figsize=(12, 6))

    for i, (bits, label) in enumerate(zip(bitstreams, labels)):
        bits = list(bits)  # Ensure bits are in list format
        indices = range(len(bits))
        plt.step(indices, [b + 2 * i for b in bits], where='mid', label=label, linewidth=2)  # Offset each stream

    plt.xlabel("Index")
    plt.title("Comparison of Data")
    plt.yticks([])  # Remove y-axis ticks since bit streams are offset
    plt.grid(True)
    plt.legend()
    plt.show()
