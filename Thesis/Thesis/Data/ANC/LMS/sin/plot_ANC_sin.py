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
# Read experiment 001
# ============================================================

filter_001 = np.fromfile(
    data_path / "adaptive_filter_001.bin",
    dtype=np.float64
)

error_001 = np.fromfile(
    data_path / "error_mu_001.bin",
    dtype=np.float64
)


# ============================================================
# Read experiment 0001
# ============================================================

filter_0001 = np.fromfile(
    data_path / "adaptive_filter_0001.bin",
    dtype=np.float64
)

error_0001 = np.fromfile(
    data_path / "error_mu_0001.bin",
    dtype=np.float64
)


# ============================================================
# Create figure
# ============================================================

fig, (ax1, ax2) = plt.subplots(
    2, 1,
    figsize=(10, 8)
)


# ============================================================
# Adaptive filter
# ============================================================

ax1.plot(
    filter_001,
    label="mu = 0.001"
)

ax1.plot(
    filter_0001,
    label="mu = 0.0001"
)

ax1.set_xlabel("Tap")
ax1.set_ylabel("Coefficient")
ax1.set_title("Adaptive Filter")
ax1.legend()
ax1.grid(True)


# ============================================================
# Error
# ============================================================

t = np.arange(len(error_001))/8000
ax2.plot(
    t,
    error_001,
    label="mu = 0.001"
)

ax2.plot(
    t,
    error_0001,
    label="mu = 0.0001"
)

ax2.set_xlabel("Sample")
ax2.set_ylabel("Error")
ax2.set_title("Error")
ax2.legend()
ax2.grid(True)


# ============================================================
# Overall title
# ============================================================

fig.suptitle("LMS Experiments")

plt.tight_layout()

# Save figure
plt.savefig(
    data_path / "lms_experiments.png",
    dpi=300,
    bbox_inches="tight"
)

plt.show()
