import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import fftconvolve


def read_binary(filename):
    try:
        with open(filename, 'rb') as file:
            return np.fromfile(file, dtype=np.int16)
    except OSError:
        raise RuntimeError(f"Error opening file: {filename}")

def FIR_filter(length, filter_buffer, filter_taps, next_sample):
    sum = 0
    for j in range(length-1, 0, -1):
        filter_buffer[j] = filter_buffer[j-1]
        sum += filter_buffer[j] * filter_taps[j]
    filter_buffer[0] = next_sample
    sum += filter_buffer[0] * filter_taps[0]

    return sum

def main():

    cancelling_length = 50
    current_delay = 0
    cancel_chamber_length = 19
    src_chamber_length = 32
    
    cancelling_delay = np.zeros(cancelling_length)
    cancelling_delay[0] = 1
    cancelling_buffer = np.zeros(cancelling_length)

    cancel_chamber_delay = np.zeros(cancel_chamber_length)
    cancel_chamber_delay[cancel_chamber_length-1] = 1
    cancel_chamber_buffer = np.zeros(cancel_chamber_length)

    src_chamber_delay = np.zeros(src_chamber_length)
    src_chamber_delay[src_chamber_length-1] = 1
    src_chamber_buffer = np.zeros(src_chamber_length)

    white_noise = read_binary("white_out_old.bin")
    #error_mic = read_binary("mic_far.bin")

    error = np.zeros(len(white_noise))
    
    energys = np.zeros(cancelling_length)
    energy_index = 0;

    for i in range(len(white_noise)):
        
        speaker2_output = FIR_filter(
            cancelling_length,
            cancelling_buffer,
            cancelling_delay,
            white_noise[i]*.5
        )

        speaker2_chamber = FIR_filter(
            cancel_chamber_length,
            cancel_chamber_buffer,
            cancel_chamber_delay,
            speaker2_output
        )

        speaker1_chamber = FIR_filter(
            src_chamber_length,
            src_chamber_buffer,
            src_chamber_delay,
            white_noise[i]*.8
        )

        error[i] = speaker1_chamber - speaker2_chamber
        energys[energy_index] += error[i]*error[i]

        if i % 1600 == 0 and i > 0:
            energy_index += 1
            cancelling_delay[:] = 0
            if current_delay + 1 < cancelling_length :
                current_delay += 1
                cancelling_delay[current_delay] = 1
            else:
                break
        

    plt.figure(figsize=(10, 5))

    plt.plot(
        error
    )

    plt.xlabel("Sample")
    plt.ylabel("Amplitude")

    plt.title(
        f"Simulated Error Signal "
    )

    plt.grid(True)

    plt.tight_layout()

    plt.figure()
    plt.plot(white_noise)
    plt.grid(True)
    plt.tight_layout()

    plt.figure()
    plt.plot(energys)
    plt.tight_layout();
    plt.grid(True)
    
    plt.show()

if __name__ == "__main__":
    main()

        

        
        
