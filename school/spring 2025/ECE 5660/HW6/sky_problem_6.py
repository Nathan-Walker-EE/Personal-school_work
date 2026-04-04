import numpy as np
import matplotlib.pyplot as plt

def generate_16qam_constellation():
    """Generate the standard 16-QAM constellation points."""
    real_part = np.array([-3, -1, 1, 3])
    imag_part = np.array([-3, -1, 1, 3])
    return np.array([r + 1j * i for r in real_part for i in imag_part])

def closest_symbol(r, constellation):
    """Find the closest constellation symbol to the received symbol r."""
    return constellation[np.argmin(np.abs(constellation - r))]

def compute_s_curve_qam():
    """Compute the S-curve for 16-QAM using a decision-directed phase detector."""
    constellation = generate_16qam_constellation()
    phase_errors = np.linspace(-180, 180, 200)  # Phase error in degrees
    s_curve = []
    
    for theta_e in np.radians(phase_errors):  # Convert degrees to radians
        sum_phase_error = 0
        for s in constellation:
            r = s * np.exp(1j * theta_e)  # Rotate symbol by phase error
            decision = closest_symbol(r, constellation)  # Decision-directed correction
            phase_diff = np.angle(r * np.conj(decision))  # Compute phase difference
            sum_phase_error += np.imag(r * np.conj(decision))  # Imaginary part is the phase detector output
        
        s_curve.append(sum_phase_error / len(constellation))  # Average over all symbols
    
    return np.radians(phase_errors), s_curve

def s_curve_bpsk(phase_error):
    return phase_error  # Restoring to a simple linear relationship as per the original correct version

def s_curve_psk(M, phase_error, K=1, A=1):
    wrapped_phase = np.mod(phase_error + np.pi / M, 2 * np.pi / M) - np.pi / M
    return K * A**2 * np.sin(wrapped_phase)

def plot_s_curve(modulation, phase_error_range=np.linspace(-np.pi, np.pi, 1000)):
    phase_error_degrees = np.degrees(phase_error_range)  # Convert to degrees
    
    if modulation == "BPSK":
        s_curve = s_curve_bpsk(phase_error_range)
    elif modulation == "8-PSK":
        s_curve = s_curve_psk(8, phase_error_range)
    elif modulation == "16-QAM":
        phase_error_range, s_curve = compute_s_curve_qam()
        phase_error_degrees = np.degrees(phase_error_range)
    else:
        raise ValueError("Unsupported modulation scheme")
    
    plt.figure()
    plt.plot(phase_error_degrees, s_curve, label=f"S-curve for {modulation}")
    plt.axhline(0, color='black', linestyle='--')
    plt.axvline(0, color='black', linestyle='--')
    plt.xlabel("Phase Error (degrees)")
    plt.ylabel("S-Curve")
    plt.title(f"S-Curve for {modulation}")
    plt.legend()
    plt.grid()
    plt.show()

# Plot S-curves for the requested modulations
plot_s_curve("BPSK")
plot_s_curve("8-PSK")
plot_s_curve("16-QAM")