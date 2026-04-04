import math
import numpy as np
from numpy.random import rand
import random
import matplotlib.pyplot as plt

def MPAM(N, Es):
    if N == 2:
        LUT = np.array([-1, 1])*Es
    elif N == 4:
        LUT = np.array([-3, -1, 1, 3])*Es
    elif N == 8:
        LUT = np.array([-7.0, -5.0, -1.0, -3.0, 7.0, 5.0, 1.0, 3.0])*Es
    
    return LUT

def CCITT(N, Es):
    if N == 8:
        ILUT = np.array([3, 1, 0, -1, -3, -1, 0, 1]) * Es
        QLUT = np.array([0, 1, 3, 1, 0, -1, -3, -1]) * Es
        return ILUT, QLUT

def compute_d_min(ILUT, QLUT):
    min_distance = float('inf')

    for i in range(len(ILUT)):
        for j in range(i + 1, len(ILUT)):
            distancex = np.abs(ILUT[i] - ILUT[j])
            distancey = np.abs(QLUT[i] - QLUT[j])
            distance = np.sqrt(distancex**2 + distancey**2)
            min_distance = min(min_distance, distance)

    return min_distance

def MQAM(N, Es):
    if N == 4:
        ILUT = np.array([1, 1, -1, -1])*Es
        QLUT = np.array([1, -1, 1, -1])*Es
    elif N == 8:
        ILUT = np.array([3, 1, -1, -3, -3, -1, 1, 3])*Es
        QLUT = np.array([1, 1, 1, 1, -1, -1, -1, -1])*Es
    return ILUT, QLUT

def MPSK(N, Es):
    if N == 8:  # 8-PSK
        ILUT = np.array([np.sqrt(Es), np.sqrt(Es)/np.sqrt(2), 0, -np.sqrt(Es)/np.sqrt(2), -np.sqrt(Es), -np.sqrt(Es)/np.sqrt(2), 0, np.sqrt(Es)/np.sqrt(2)])  # In-phase component
        QLUT = np.array([0, np.sqrt(Es)/np.sqrt(2), np.sqrt(Es), np.sqrt(Es)/np.sqrt(2), 0, -np.sqrt(Es)/np.sqrt(2), -np.sqrt(Es), -np.sqrt(Es)/np.sqrt(2)]) # Quadrature component
        #gray-coded
        ILUT = np.array([np.sqrt(Es), np.sqrt(Es)/np.sqrt(2),-np.sqrt(Es)/np.sqrt(2), 0, np.sqrt(Es)/np.sqrt(2), 0, -np.sqrt(Es), -np.sqrt(Es)/np.sqrt(2)])
        QLUT = np.array([0, np.sqrt(Es)/np.sqrt(2),np.sqrt(Es)/np.sqrt(2), np.sqrt(Es), -np.sqrt(Es)/np.sqrt(2), -np.sqrt(Es), 0, -np.sqrt(Es)/np.sqrt(2)])
    return ILUT, QLUT

def QAM_dec(N, Es, ILUT, QLUT, rx, ry):
    if N == 4:
        if rx > 0:
            xhat = 1
        else:
            xhat = -1
        if ry > 0:
            yhat = 1
        else:
            yhat = -1
        if xhat == 1:
            if yhat == 1:
                rinteger = 0
            else:
                rinteger = 1
        else:
            if yhat == 1:
                rinteger = 2
            else:
                rinteger = 3
        return int(rinteger)
    elif N == 8:
        if ry > 0:
            if rx > 2:
                rinteger = 0
            elif rx < 2 and rx > 0:
                rinteger = 1
            elif rx < 0 and rx > -2:
                rinteger = 2
            elif rx < -2:
                rinteger = 3
        elif ry < 0:
            if rx > 2:
                rinteger = 7
            elif rx < 2 and rx > 0:
                rinteger = 6
            elif rx < 0 and rx > -2:
                rinteger = 5
            elif rx < -2:
                rinteger = 4
        return int(rinteger)

def PAM_dec(N, Es, r):
    if N == 2:
        if r > 0:
            shat = Es
        else:
            shat = -Es
    elif N == 8:
        if r > 0:
            if r < 2 * Es:
                shat = 1.0
            elif r > 2 * Es and r < 4 * Es:
                shat = 3.0
            elif r > 4 * Es and r < 6 * Es:
                shat = 5.0
            else:
                shat = 7.0
        if r < 0:
            if r > -2 * Es:
                shat = -1.0
            elif r < -2 * Es and r > -4 * Es:
                shat = -3.0
            elif r < -4 * Es and r > -6 * Es:
                shat = -5.0
            else:
                shat = -7.0
    return shat*Es

def PSK_dec(N, Es, ILUT, QLUT, rx, ry):
    if N == 8:
        distances = np.sqrt((ILUT - rx) ** 2 + (QLUT - ry) ** 2)  # Compute Euclidean distances
        closest_index = np.argmin(distances)  # Get index of minimum distance
        return int(closest_index)