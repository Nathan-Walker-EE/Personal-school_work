import math
import numpy as np
import matplotlib.pyplot as plt

def readInputFile(filename):
    with open(filename, 'r') as f:
        data = [float(line.strip()) for line in f if line.strip()]
    return data

def find_lut_index_xy(x, y, lut):
    diffs = lut - np.array([x, y])  # Shape: (N, 2)
    distances = np.linalg.norm(diffs, axis=1)  # Euclidean distance
    index = np.argmin(distances)
    return index, lut[index]

def drawKitties(data, colsize, simname):
    N = len(data)
    # Compute the number of columns (ceiling division)
    num_columns = -(-N // colsize)  # equivalent to math.ceil(N/colsize)
    total_pixels = num_columns * colsize
    missing = total_pixels - N
    
    # Prepend zeros so that the final data point goes to bottom-right
    complete_data = [0] * missing + data

    # Create a 2D image with 'colsize' rows and 'num_columns' columns.
    image = [[0 for _ in range(num_columns)] for _ in range(colsize)]
    # Fill the image in column-major order.
    for col in range(num_columns):
        for row in range(colsize):
            index = col * colsize + row
            image[row][col] = complete_data[index]
            
    # Display the image.
    plt.figure()
    plt.title(f"{simname}: kitten with UW")
    plt.imshow(image, cmap='gray', vmin=0, vmax=255)
    plt.axis('off')
    plt.show()


def getLUT():
    B = 8;            # Bits per symbol (B should be even: 8, 6, 4, 2)
    # B = 4;
    bits2index = 2**np.arange(B-1,-1,-1)
    M = 2 ** B       # Number of symbols in the constellation
    Mroot = math.floor(2**(B/2))
    a = np.reshape(np.arange(-Mroot+1,Mroot,2),(2*B,1))
    b = np.ones((Mroot,1))
    LUT = np.hstack((np.kron(a,b), np.kron(b,a)))
    Enorm = np.sum(LUT ** 2) / M;
    LUT = LUT/math.sqrt(Enorm);

    return LUT, Enorm


def srrc1(alpha, N, Lp):
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
            num = math.sin(math.pi * (1 - alpha) * n/N)
            num += (4 * alpha * n) / N * math.cos(math.pi * (1 + alpha) * n/N)
            denom = 4 * alpha * n / N
            denom = 1 - denom * denom
            denom = denom * math.pi * n / N
            sample = (1 / math.sqrt(N)) * num / denom        
        srrc_samples[n + Lp] = sample
    return srrc_samples 


def mapToConstellation(x, y, LUT):
    LUT = np.asarray(LUT)
    
    # Ensure inputs are arrays, even for scalar inputs
    x = np.atleast_1d(x)
    y = np.atleast_1d(y)
    
    # Combine x and y values into array of points (N, 2)
    points = np.column_stack((x, y))
    
    # Compute squared Euclidean distance from each point to each LUT entry
    distances = np.sum((points[:, np.newaxis, :] - LUT[np.newaxis, :, :]) ** 2, axis=2)
    
    # Find index of nearest LUT point
    indices = np.argmin(distances, axis=1)
    
    # Get quantized (closest) points
    quantized_points = LUT[indices]
    quantized_x = quantized_points[:, 0]
    quantized_y = quantized_points[:, 1]
    
    # If the input was scalar, return scalars instead of arrays
    if quantized_x.size == 1:
        return quantized_x[0], quantized_y[0], indices[0]
    else:
        return quantized_x, quantized_y, indices

def getUniqueWord(LUT, Enorm=1):
    # Unique word (randomly generated)
    uw = np.array([162,29,92,47,16,112,63,234,50,7,15,211,109,124,
                   239,255,243,134,119,40,134,158,182,0,101,
                   62,176,152,228,36])
    uw_len = uw.size
    uwsym = LUT[uw,:]

    # Build the list of four possible UW rotations
    angles = 2*math.pi*np.arange(0,4)/4
    uwrotsyms = np.zeros((uw_len,2,4))
    for i in range(angles.size):
        C = math.cos(angles[i])
        S = -math.sin(angles[i])
        G = np.array([[C, -S],[S, C]])
        uwrot = uwsym @ G
        uwrotsyms[:,:,i] = uwrot

    return uwrotsyms

def farrow_cubic_interp(FX, mu):
    vF3 = FX[3]/6 + FX[2]/-2 + FX[1]/2 + FX[0]/-6
    vF2 = FX[2]/2 - FX[1] + FX[0]
    vF1 = FX[3]/-6 + FX[2] + FX[1]/-2 + FX[0]/-3
    vF0 = FX[1]
    return ((mu*vF3+vF2)*mu + vF1)*mu + vF0

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

def slicer_256qam(symbol, LUT):
    # print(LUT)
    dists = np.abs(LUT - symbol)
    return LUT[np.argmin(dists)]

def phase_tracking_PLL(symbols, LUT, k1, k2):
    N = len(symbols)
    phase = 0.0
    x1 = 0.0
    x2 = 0.0
    output = np.zeros_like(symbols, dtype=complex)
    e_buf = []
    
    for i in range(N):
        corrected = symbols[i] * np.exp(-1j * phase)
        decision = slicer_256qam(corrected, LUT)
        error = np.imag(corrected * np.conj(decision))
        x1 = error * k2 + x2
        v = error * k1 + x1
        x2 = x1
        phase += v
        output[i] = corrected
        e_buf.append(error)
    
    plt.plot(e_buf)
    plt.show()
    return output

def fix_image_shift(data, colsize, shift_rows):
    N = len(data)
    num_columns = -(-N // colsize)  # ceiling division
    total_pixels = num_columns * colsize
    missing = total_pixels - N
    padded_data = [0] * missing + data

    image = np.array(padded_data).reshape((num_columns, colsize)).T  # shape: (colsize, num_columns)
    shifted_image = np.roll(image, shift_rows, axis=0)  # shift rows
    fixed_data = shifted_image.T.flatten().tolist()
    return fixed_data

import matplotlib.pyplot as plt

def plot_scatter(i_samples, q_samples):
    plt.figure(figsize=(6, 6))
    plt.scatter(i_samples, q_samples, s=2, alpha=0.6)
    plt.title('received constellation')
    plt.xlabel("In-phase (I)")
    plt.ylabel("Quadrature (Q)")
    plt.grid(True)
    plt.axis("equal")  # Keep aspect ratio 1:1
    plt.tight_layout()
    plt.show()

def plot_error_signal(error_signal, title):
    plt.figure()
    plt.plot(error_signal, label='Error', color='tab:red')
    plt.title(title)
    plt.xlabel("Sample Index")
    plt.ylabel("Error Value")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

def plot_phase_signal(phase_signal):
    title="Recovered Phase Signal"
    label="Phase"
    plt.figure()
    plt.plot(phase_signal, label=label, color='tab:blue')
    plt.title(title)
    plt.xlabel("Sample Index")
    plt.ylabel("Phase (radians)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

def plot_fractional_interval(mu_values):
    title="Fractional Interval (mu) Over Time"
    label="mu"
    plt.figure()
    plt.plot(mu_values, label=label, color='tab:green')
    plt.title(title)
    plt.xlabel("Sample Index")
    plt.ylabel("Fractional Interval (mu)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()