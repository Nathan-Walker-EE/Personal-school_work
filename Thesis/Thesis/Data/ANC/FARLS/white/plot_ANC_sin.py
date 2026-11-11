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

Fs = 8000


# ============================================================
# Read experiment - Input Range = 1.0
# ============================================================

filter_1 = np.fromfile(
    data_path / "adaptive_filter_1.bin",
    dtype=np.float64
)

error_1 = np.fromfile(
    data_path / "error_1.bin",
    dtype=np.float64
)


# ============================================================
# Read experiment - Input Range = 0.75
# ============================================================

filter_075 = np.fromfile(
    data_path / "adaptive_filter_75.bin",
    dtype=np.float64
)

error_075 = np.fromfile(
    data_path / "error_75.bin",
    dtype=np.float64
)


# ============================================================
# Read experiment - Input Range = 0.5
# ============================================================

filter_05 = np.fromfile(
    data_path / "adaptive_filter_5.bin",
    dtype=np.float64
)

error_05 = np.fromfile(
    data_path / "error_5.bin",
    dtype=np.float64
)


# ============================================================
# Create time vectors
# ============================================================

t_1 = np.arange(len(error_1)) / Fs
t_075 = np.arange(len(error_075)) / Fs
t_05 = np.arange(len(error_05)) / Fs


# ============================================================
# Create figure
# ============================================================

fig, (ax1, ax2) = plt.subplots(
    2, 1,
    figsize=(10, 8)
)


# ============================================================
# Adaptive filters
# ============================================================

ax1.plot(
    filter_1,
    label="Input Range = 1.0"
)

ax1.plot(
    filter_075,
    label="Input Range = 0.75"
)

ax1.plot(
    filter_05,
    label="Input Range = 0.5"
)

ax1.set_xlabel("Tap")
ax1.set_ylabel("Coefficient")
ax1.set_title("Adaptive Filter")
ax1.legend()
ax1.grid(True)


# ============================================================
# Error
# ============================================================

ax2.plot(
    t_1,
    error_1,
    label="Input Range = 1.0"
)

ax2.plot(
    t_075,
    error_075,
    label="Input Range = 0.75"
)

ax2.plot(
    t_05,
    error_05,
    label="Input Range = 0.5"
)

ax2.set_xlabel("Time (seconds)")
ax2.set_ylabel("Error")
ax2.set_title("Error")
ax2.legend()
ax2.grid(True)


# ============================================================
# Overall title
# ============================================================

fig.suptitle("FARLS Experiments")

plt.tight_layout()


# ============================================================
# Save figure
# ============================================================

plt.savefig(
    data_path / "FARLS_experiments_all_ranges.png",
    dpi=300,
    bbox_inches="tight"
)


# ============================================================
# Show
# ============================================================

plt.show()
