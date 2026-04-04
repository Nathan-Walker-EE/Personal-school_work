import numpy as np
import math
import matplotlib.pyplot as plt
import useful_functions as funct

#----------------------#
#--    Parameters    --#
#----------------------#
Ts = 1                              # sample period
N = 4                               # samples per symbol
alpha = 0.7                         # excess bandwidth
Lp = 12                             # pulse length
srrc = funct.srrc1(alpha, N, Lp)    # pulse
w0 = math.pi/2                      # carrier frequency

#--------------------------------------#
#--    Generate the Look-Up Table    --#
#--------------------------------------#
LUT, Enorm = funct.getLUT()

#---------------------------------------#
#--    Read in the Received Signal    --#
#---------------------------------------#
filename = 'sim3_2025'
r = funct.readInputFile(filename)
r = np.array(r)

#---------------------------------#
#--    Demodulate the Signal    --#
#---------------------------------#
n = np.arange(r.size)
cosine = math.sqrt(2)*np.cos(w0*n)
sine = math.sqrt(2)*np.sin(w0*n)
I = r * cosine
Q = r * sine * (-1)

#--------------------------------#
#--    Apply Matched Filter    --#
#--------------------------------#
I_x = np.convolve(I, srrc, mode= 'same')
Q_x = np.convolve(Q, srrc, mode= 'same')

#---------------------------------------#
#--    Generate Downsampled Signal    --#
#---------------------------------------#
x = []
y = []
cnt = N 
for n in range(Lp*4 - (N - 2), len(I_x) - Lp*4):
    cnt -= 1
    if cnt == 0:
        cnt = N
        x.append(I_x[n])
        y.append(Q_x[n])
        if (I_x[n] < 0.05 and I_x[n] > -0.05 and Q_x[n] < 0.05 and Q_x[n] > -0.05):
            print("It is 0 const point at index ", len(x), " which is time ", n)

#-------------------------------------#
#--    Phase Tracking               --#
#-------------------------------------#
# Convert x/y to complex symbols
symbols = np.array(x) + 1j * np.array(y)

# Convert LUT to complex 1D
LUT_complex = LUT[:, 0] + 1j * LUT[:, 1]

# Loop filter coefficients (tune as needed)
zeta = 1/np.sqrt(2)
Bn = 0.05
T = 1
kp = 1
k0 = 1

k1 = funct.compute_K1(Bn, T, zeta, kp, k0)
k2 = funct.compute_K2(Bn, T, zeta, kp, k0)
print(k1, k2)

# Run PLL
tracked_symbols = funct.phase_tracking_PLL(symbols, LUT_complex, k1, k2)
x_corrected = tracked_symbols.real
y_corrected = tracked_symbols.imag

# rotate 90 degrees
tracked_symbols_rot90 = tracked_symbols * 1j
x_rot90 = tracked_symbols_rot90.real
y_rot90 = tracked_symbols_rot90.imag

# rotate 90 degrees again (180 total)
tracked_symbols_rot180 = tracked_symbols_rot90 * 1j
x_rot180 = tracked_symbols_rot180.real
y_rot180 = tracked_symbols_rot180.imag

# rotate 90 degrees again (270 total)
tracked_symbols_rot270 = tracked_symbols_rot180 * 1j
x_rot270 = tracked_symbols_rot270.real
y_rot270 = tracked_symbols_rot270.imag

#----------------------------------#
#--    Map onto Constellation    --#
#----------------------------------#
constx, consty, points = funct.mapToConstellation(x_corrected,y_corrected,LUT)
constx, consty, points90 = funct.mapToConstellation(x_rot90,y_rot90,LUT)
constx, consty, points180 = funct.mapToConstellation(x_rot180,y_rot180,LUT)
constx, consty, points270 = funct.mapToConstellation(x_rot270,y_rot270,LUT)

#-----------------------#
#--    Plot Kitten    --#
#-----------------------#
funct.drawKitties(points.tolist(), 130, filename)
plt.show()
funct.drawKitties(points90.tolist(), 130, filename)
plt.show()
funct.drawKitties(points180.tolist(), 130, filename)
plt.show()
funct.drawKitties(points270.tolist(), 130, filename)
plt.show()

plt.figure()
#plt.scatter(x, y, label="Before Rotation")
plt.scatter(x_corrected, y_corrected, label="After Rotation")
plt.axis('equal')
plt.grid(True)
plt.legend()
#plt.title("Constellation Before/After Phase Correction")
plt.title("No UW correction")

plt.figure()
plt.scatter(x_rot90, y_rot90, label="After Rotation")
plt.axis('equal')
plt.grid(True)
plt.title("90 UW correction")

plt.figure()
plt.scatter(x_rot180, y_rot180, label="After Rotation")
plt.axis('equal')
plt.grid(True)
plt.title("180 UW correction")

plt.figure()
plt.scatter(x_rot270, y_rot270, label="After Rotation")
plt.axis('equal')
plt.grid(True)
plt.title("270 UW correction")

plt.show()