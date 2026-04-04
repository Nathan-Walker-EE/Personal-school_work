import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt

def srrc_pulse(N, Lp, Ts, alpha):
    p = np.zeros(2 * Lp + 1)
    for n in range(-Lp, Lp + 1):
        t = n / N  # Normalize n by samples per symbol
        if t == 0:
            p[n + Lp] = 1 / np.sqrt(N) * (1 + alpha * (4 / np.pi - 1))
        elif abs(t) == 1 / (4 * alpha):
            p[n + Lp] = (alpha / (np.sqrt(2 * N))) * \
                ( (1 + 2 / np.pi) * np.sin(np.pi / (4 * alpha)) +
                  (1 - 2 / np.pi) * np.cos(np.pi / (4 * alpha)) )
        else:
            numerator = np.sin(np.pi * (1 - alpha) * t) + \
                        4 * alpha * t * np.cos(np.pi * (1 + alpha) * t)
            denominator = (np.pi * t) * (1 - (4 * alpha * t) ** 2)
            p[n + Lp] = (1 / np.sqrt(N)) * (numerator / denominator)
    return p