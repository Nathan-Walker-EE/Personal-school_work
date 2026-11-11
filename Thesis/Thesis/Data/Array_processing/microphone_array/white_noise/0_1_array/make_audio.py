import numpy as np
from scipy.io import wavfile
import sys


def bin_to_wav(input_file, output_file, sample_rate=8000):
    # Read binary file as 64-bit floating-point values
    data = np.fromfile(input_file, dtype=np.float64)

    # Select only 7.5 to 12.5 seconds
    start_time = 0
    end_time = len(data)/sample_rate

    start_sample = int(start_time * sample_rate)
    end_sample = int(end_time * sample_rate)

    if end_sample > len(data):
        print("Error: BIN file is shorter than 12.5 seconds.")
        sys.exit(1)

    data = data[start_sample:end_sample]

    print(f"Read samples from {start_time} to {end_time} seconds")
    print(f"Number of samples: {len(data)}")
    print(f"Duration: {len(data) / sample_rate:.2f} seconds")
    print(f"Min: {np.min(data)}")
    print(f"Max: {np.max(data)}")

    # Normalize to [-1, 1]
    max_value = np.max(np.abs(data))

    if max_value > 0:
        data = data / max_value

    # Convert to 16-bit PCM
    data_int16 = (data * 32767).astype(np.int16)

    # Write WAV file
    wavfile.write(output_file, sample_rate, data_int16)

    print(f"Created: {output_file}")


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage:")
        print("    python bin_to_wav.py input.bin output.wav")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    bin_to_wav(input_file, output_file)


