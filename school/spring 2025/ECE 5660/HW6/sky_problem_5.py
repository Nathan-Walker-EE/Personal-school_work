import numpy as np
import matplotlib.pyplot as plt

def differential_encode(bits):
    phase_mapping = {
        (0, 0): 180,
        (0, 1): -90,
        (1, 0): 0,
        (1, 1): 90,
    }
    encoded = []
    prev_phase = 0  # Assume initial phase is 0
    for i in range(0, len(bits), 2):
        bit_pair = (bits[i], bits[i+1])
        phase_shift = phase_mapping[bit_pair]
        prev_phase = (prev_phase + phase_shift) % 360  # Keep within [0, 360)
        encoded.append(np.exp(1j * np.deg2rad(prev_phase)))
    return np.array(encoded)

def differential_decode(symbols):
    phase_mapping = {
        180: (0, 0), -180: (0, 0),
        -90: (0, 1), 270: (0, 1),
        0: (1, 0), 360: (1, 0),
        90: (1, 1), -270: (1, 1)
    }
    decoded = []
    prev_phase = 0  # Assume initial phase is 0
    possible_keys = np.array(list(phase_mapping.keys()))
    for symbol in symbols:
        phase = np.rad2deg(np.angle(symbol)) % 360  # Normalize to [0, 360)
        phase_diff = ((phase - prev_phase + 180) % 360) - 180  # Normalize to [-180, 180]
        
        # Find the closest valid key
        closest_key = possible_keys[np.argmin(np.abs(possible_keys - phase_diff))]
        decoded.extend(phase_mapping[closest_key])
        prev_phase = phase
    return np.array(decoded)

def test_differential_qpsk():
    bits = np.random.randint(0, 2, 20)  # Generate random bit sequence
    encoded = differential_encode(bits)
    
    for phase_offset in [0, 90, 180, 270]:
        phase_rad = np.deg2rad(phase_offset)
        rotated_symbols = encoded * np.exp(1j * phase_rad)
        decoded = differential_decode(rotated_symbols)
        
        print(f"Phase Offset: {phase_offset} degrees")
        print(f"Original:  {bits}")
        print(f"Decoded:   {decoded}")

test_differential_qpsk()