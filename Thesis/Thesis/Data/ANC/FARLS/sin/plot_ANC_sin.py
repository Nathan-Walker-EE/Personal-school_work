import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path

plt.rcParams.update({
    'font.size': 18,
    'axes.titlesize': 20,     # Make subplot titles slightly larger
    'figure.titlesize': 22,   # Make the main suptitle stand out
    'legend.fontsize': 12     # Shrink legend font slightly so it doesn't block data
})

# ============================================================
# Data path
# ============================================================

data_path = Path(__file__).resolve().parent


# ============================================================
# Read experiment 75
# ============================================================

filter_75 = np.fromfile(
    data_path / "adaptive_filter_75.bin",
    dtype=np.float64
)

error_75 = np.fromfile(
    data_path / "error_75.bin",
    dtype=np.float64
)


# ============================================================
# Read experiment 5
# ============================================================

filter_5 = np.fromfile(
    data_path / "adaptive_filter_5.bin",
    dtype=np.float64
)

error_5 = np.fromfile(
    data_path / "error_5.bin",
    dtype=np.float64
)


# ============================================================
# Read experiment 25
# ============================================================

filter_25 = np.fromfile(
    data_path / "adaptive_filter_25.bin",
    dtype=np.float64
)

error_25 = np.fromfile(
    data_path / "error_25.bin",
    dtype=np.float64
)


# ============================================================
# Create time vectors
# ============================================================

t_75 = np.arange(len(error_75)) / 8000
t_5 = np.arange(len(error_5)) / 8000
t_25 = np.arange(len(error_25)) / 8000


# ============================================================
# FIGURE 1 - mu = 0.001
# ============================================================

fig1, (ax1, ax2) = plt.subplots(
    2, 1,
    figsize=(10, 8)
)


# Adaptive filter - 75
ax1.plot(
    filter_75
)

ax1.set_xlabel("Tap")
ax1.set_ylabel("Coefficient")
ax1.set_title("Adaptive Filter")
ax1.grid(True)


# Error - 75
ax2.plot(
    t_75,
    error_75
)

ax2.set_xlabel("Time (seconds)")
ax2.set_ylabel("Error")
ax2.set_title("Error")
ax2.grid(True)


fig1.suptitle("FARLS Experiment - Input Range = .75")

plt.tight_layout()

plt.savefig(
    data_path / "FARLS_experiment_75.png",
    dpi=300,
    bbox_inches="tight"
)


# ============================================================
# FIGURE 2 - mu = 0.0001 and mu = 0.00001
# ============================================================

fig2, (ax3, ax4) = plt.subplots(
    2, 1,
    figsize=(10, 8)
)


# Adaptive filters - 5 and 25
ax3.plot(
    filter_5,
    label="Input Range = 0.5"
)

ax3.plot(
    filter_25,
    label="Input Range = 0.25"
)

ax3.set_xlabel("Tap")
ax3.set_ylabel("Coefficient")
ax3.set_title("Adaptive Filter")
ax3.legend()
ax3.grid(True)


# Errors - 5 and 25
ax4.plot(
    t_5,
    error_5,
    label="Input Range = 0.5"
)

ax4.plot(
    t_25,
    error_25,
    label="Input Range = 0.25"
)

ax4.set_xlabel("Time (seconds)")
ax4.set_ylabel("Error")
ax4.set_title("Error")
ax4.legend()
ax4.grid(True)


fig2.suptitle("FARLS Experiments - Small Input Ranges")

plt.tight_layout()

plt.savefig(
    data_path / "FARLS_experiments_small_ranges.png",
    dpi=300,
    bbox_inches="tight"
)


# ============================================================
# Show both figures
# ============================================================

plt.show()
