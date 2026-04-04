import math
import numpy as np
import matplotlib.pyplot as plt


# read input file
def readInputFile(filename):
    with open(filename, 'r') as f:
        data = [float(line.strip()) for line in f if line.strip()]
    return data

def drawKitties(data, colsize):
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
    plt.title("kittyyyy")
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
    print(LUT)
    # will be of the form (for example)
    #  -3, -3
    #  -3, -1
    #  -3, 1
    #  ...
    #   3, 3
    # of shape (B^2, 2)

    # Scale the constellation to have unit energy
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


def matchedFilter(s, pulse_shape):
    s = np.convolve(s, pulse_shape, mode='same')
    #s = np.convolve(s, pulse_shape)
    return s

def mapToConstellation(x, y, LUT):
    LUT = np.asarray(LUT)
    # Ensure inputs are numpy arrays
    x = np.asarray(x)
    y = np.asarray(y)
    
    # Combine x and y values into an array of points (N, 2)
    points = np.column_stack((x, y))
    
    # Calculate the squared Euclidean distance from each received point to each LUT point
    distances = np.sum((points[:, np.newaxis, :] - LUT[np.newaxis, :, :]) ** 2, axis=2)
    
    # Find the index of the closest LUT point for each received point
    indices = np.argmin(distances, axis=1)
    
    # Retrieve the quantized points from the LUT
    quantized_points = LUT[indices]
    quantized_x = quantized_points[:, 0]
    quantized_y = quantized_points[:, 1]
    
    return quantized_x, quantized_y, indices


def computeK(Bn, T, zeta, kp, k0):
    num1 = (4*zeta*Bn*T)/(zeta+1/(4*zeta))
    num2 = 4*((Bn*T)/(zeta + 1/(4*zeta)))**2
    den = 1 + (2*zeta*Bn*T)/(zeta+1/(4*zeta)) + ((Bn*T)/(zeta+1/(4*zeta)))**2
    k1 = num1/den/kp/k0; k2 = num2/den/kp/k0
    return k1, k2


def getUniqueWord(LUT, Enorm):
    # Unique word (randomly generated)
    uw = np.array([162,29,92,47,16,112,63,234,50,7,15,211,109,124,
                    239,255,243,134,119,40,134,158,182,0,101,
                    62,176,152,228,36])
    uw_len = uw.size
    uw = uw.reshape(uw_len,)

    uwsym = LUT[uw,:]


    # Build the list of four possible UW rotations
    angles = 2*math.pi*np.arange(0,4)/4
    print(angles)
    uwrotsyms = np.zeros((uw_len,2,4));

    for i in range(angles.size):
        C = math.cos(angles[i])
        S = -math.sin(angles[i])
        G = np.array([[C, -S],[S, C]])
        uwrot = uwsym @ G;  # Rotate the UW symbols
        uwrotsyms[:,:,i] = uwrot; # Save the rotated version

    print(LUT* np.sqrt(Enorm))
    #print(uwrotsyms)

