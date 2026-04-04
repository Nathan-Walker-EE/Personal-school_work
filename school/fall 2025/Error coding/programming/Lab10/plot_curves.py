import numpy as np
import matplotlib.pyplot as plt

# --------------------------------------
# Load single-row CSV arrays
# --------------------------------------
def load_curve(filename):
    # loads a 1-row CSV: v0, v1, ..., vN
    arr = np.loadtxt(filename, delimiter=",")
    # Ensure arr is 1D (in case numpy gives a 2D array)
    arr = np.ravel(arr)
    return arr

# --------------------------------------
# Load your curves
# --------------------------------------
uncoded = load_curve("uncoded1_per_curve.csv")
hard    = load_curve("hard1_per_curve.csv")
soft    = load_curve("soft1_per_curve.csv")

# --------------------------------------
# Build SNR axis automatically
# --------------------------------------
# Example: if your C++ loop does:
#   for (int i = start_db; i < end_db; i++)
# then Python infers the same x-axis:
start_db = 0     # <--- CHANGE if your C++ uses a different start
end_db   = start_db + len(uncoded)

EbN0_dB = np.arange(start_db, end_db)

# --------------------------------------
# Plot
# --------------------------------------
plt.figure(figsize=(8,6))

plt.semilogy(EbN0_dB, uncoded, 'o-', label="Uncoded BPSK")
plt.semilogy(EbN0_dB, hard,    's-', label="Hard-decision Viterbi")
plt.semilogy(EbN0_dB, soft,    '^-', label="Soft-decision Viterbi")

plt.xlabel("Eb/N0 (dB)")
plt.ylabel("BER / PER")
plt.grid(True, which="both", linestyle="--")
plt.legend()
plt.title("Uncoded vs Hard vs Soft BER/PER")

plt.tight_layout()
plt.show()
