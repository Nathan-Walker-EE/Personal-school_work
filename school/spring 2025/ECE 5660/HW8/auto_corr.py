import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt

c = np.array([1, 1, 1, -1, 1, 1, -1, -1, 1,
               -1, 1, -1, -1, -1, -1])

r = np.zeros(2*len(c) -1)

for tau in range(len(r)):
    r[tau] = 0
    for i in range(len(c)):
        if i - tau >= 0:
            index = i - tau
        elif i - tau < 0:
            index = i - tau + len(c)
        r[tau] += c[i]*c[index]
    r[tau] = r[tau]/len(c)

plt.stem(r)
plt.show()