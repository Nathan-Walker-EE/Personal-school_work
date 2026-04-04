import numpy as np
import matplotlib.pyplot as plt

def generate_16qam_constellation():
    """Generate 16-QAM constellation points."""
    points = np.array([-3, -1, 1, 3])
    real, imag = np.meshgrid(points, points)  # Create a grid
    return (real + 1j * imag).flatten()

def closest_qam_symbol(received, constellation):
    """Find the nearest 16-QAM constellation point to the received symbol."""
    return constellation[np.argmin(np.abs(constellation - received))]

def compute_s_curve_qam():
    """Compute the decision-directed S-curve for 16-QAM."""
    constellation = generate_16qam_constellation()
    phase_errors = np.radians(np.linspace(-180, 180, 200))  # Convert degrees to radians
    s_curve = np.zeros_like(phase_errors)

    for i, theta_e in enumerate(phase_errors):
        error_sum = sum(
            np.imag((s * np.exp(1j * theta_e)) * np.conj(closest_qam_symbol(s * np.exp(1j * theta_e), constellation)))
            for s in constellation
        )
        s_curve[i] = error_sum / len(constellation)

    return phase_errors, s_curve

def s_curve_bpsk(phase_errors):
    """S-curve for BPSK (linear relationship)."""
    return phase_errors

def s_curve_psk(M, phase_errors, K=1, A=1):
    """Compute the S-curve for M-PSK."""
    wrapped_phase = np.mod(phase_errors + np.pi / M, 2 * np.pi / M) - np.pi / M
    return K * A**2 * np.sin(wrapped_phase)

def plot_s_curve(modulation):
    """Plot S-curves for BPSK, M-PSK, and 16-QAM."""
    phase_errors = np.linspace(-np.pi, np.pi, 1000)  # Phase errors in radians
    phase_errors_degrees = np.degrees(phase_errors)  # Convert to degrees

    if modulation == "BPSK":
        s_curve = s_curve_bpsk(phase_errors)
    elif modulation == "8-PSK":
        s_curve = s_curve_psk(8, phase_errors)
    elif modulation == "16-QAM":
        phase_errors, s_curve = compute_s_curve_qam()
        phase_errors_degrees = np.degrees(phase_errors)  # Convert back to degrees
    else:
        raise ValueError("Unsupported modulation scheme.")

    plt.plot(phase_errors_degrees, s_curve, label=f"S-Curve for {modulation}")
    plt.axhline(0, color='black', linestyle='--')
    plt.axvline(0, color='black', linestyle='--')
    plt.xlabel("Phase Error (degrees)")
    plt.ylabel("S-Curve")
    plt.title(f"S-Curve for {modulation}")
    plt.legend()
    plt.show()

# Plot S-curves for the requested modulations
for mod in ["BPSK", "8-PSK", "16-QAM"]:
    plot_s_curve(mod)
