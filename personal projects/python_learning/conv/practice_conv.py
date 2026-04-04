import numpy as np 
import matplotlib.pyplot as plt 
import scipy
from scipy import signal 
from scipy.io import loadmat
import sys
import os
sys.path.insert(0, "../function_files")
from time_domain import conv
import math

Fs = 400
f = 115
n = np.arange(0,3*Fs)

x = np.cos(2 * np.pi * (f/Fs) * n)
print(len(n), len(x))

# mat_data = loadmat('filter.mat')
# h = mat_data['h'].flatten()
h = np.arange(1,5)
m = np.arange(0, len(x) + len(h) - 1)

y = conv(x,h)
print(len(y), len(m))

plt.figure(figsize = (6,4))
plt.plot(n, x)
plt.show()

plt.figure(figsize = (6,4))
plt.plot(m, y)
plt.show()