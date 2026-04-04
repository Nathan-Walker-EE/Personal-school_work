import numpy as np

# Differential QPSK table: phase shift (in degrees) → bits
phase_to_bits = {
    180: '00',
    -90: '01',
    0:   '10',
    90:  '11'
}

# Your IQ sequence
iq_sequence = [(1,-1), (-1,-1), (1,1), (-1,1), (-1,-1), 
               (1,1), (-1,-1), (-1, -1), (-1,1), (1,-1), 
               (1,1), (-1,-1), (1, -1)]

# Convert to complex symbols
z = np.array([complex(i, q) for i, q in iq_sequence])

# Compute phase differences in degrees
phase_diffs = np.angle(z[1:] / z[:-1], deg=True)

# Quantize to nearest of -90, 0, 90, 180
def quantize_phase(phase):
    options = np.array([-90, 0, 90, 180])
    idx = np.argmin(np.abs(phase - options))
    return options[idx]

# Map to bits
decoded_bits = []
for k, raw_phase in enumerate(phase_diffs):
    phase = quantize_phase(raw_phase)
    bits = phase_to_bits[phase]
    decoded_bits.append(bits)
    print(f"Δθ_{k+1}: {raw_phase:.1f}° → {phase}° → bits: {bits}")

# Optional: combine bits into a string
bitstream = ''.join(decoded_bits)
print("\nDecoded bitstream:", bitstream)
