import numpy as np 

def conv(x, h):
    length_x = len(x)
    length_h = len(h)

    xzp = np.zeros(length_x + 2*(length_h-1))
    for i in range(length_x):
        xzp[i + length_h] = x[i]

    M = length_h + length_x - 1
    y = np.zeros(M)

    for i in range(M-1): 
        for k in range(length_h-1):
            y[i] = y[i] + xzp[i - k + length_h] * h[k]

    return y