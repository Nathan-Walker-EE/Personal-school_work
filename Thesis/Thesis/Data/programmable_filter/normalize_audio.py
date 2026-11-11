import os
import numpy as np
import pydub

def normalize_m4a_to_wav(input_path, output_path):
    """
    Reads an M4A file, normalizes its audio samples to the range [-1.0, 1.0],
    and saves it as a WAV file.
    """
    print(f"Processing: {input_path}")
    
    # Load M4A file using pydub
    audio = pydub.AudioSegment.from_file(input_path, format="m4a")
    
    # Convert raw data to a NumPy array based on sample width
    samples = np.array(audio.get_array_of_samples(), dtype=np.float32)
    
    # Find the maximum absolute value to normalize against peak
    max_val = np.max(np.abs(samples))
    
    if max_val > 0:
        # Scale samples to peak at exactly 1.0 (range -1.0 to 1.0)
        normalized_samples = samples / max_val
    else:
        normalized_samples = samples

    # Convert back to standard 16-bit PCM integers for the output WAV
    # (Scale by 32767 to fill the int16 dynamic range)
    int16_samples = (normalized_samples * 32767).astype(np.int16)
    
    # Create a new AudioSegment from the normalized array data
    normalized_audio = audio._spawn(int16_samples.tobytes())
    
    # Export as a standard WAV file
    normalized_audio.export(output_path, format="wav")
    print(f"Saved normalized file to: {output_path}")

def batch_process_m4a(input_dir, output_dir):
    """
    Finds all .m4a files in the input directory, normalizes them,
    and writes them out as .wav files to the output directory.
    """
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        
    files = [f for f in os.listdir(input_dir) if f.lower().endswith(".m4a")]
    
    if not files:
        print(f"No .m4a files found in '{input_dir}'")
        return
        
    for file in files:
        in_file = os.path.join(input_dir, file)
        out_file = os.path.join(output_dir, os.path.splitext(file)[0] + ".wav")
        normalize_m4a_to_wav(in_file, out_file)

if __name__ == "__main__":
    # Configure your directories here
    INPUT_DIRECTORY = "./input_audio"
    OUTPUT_DIRECTORY = "./output_audio"
    
    # Create a dummy input folder for user convenience
    if not os.path.exists(INPUT_DIRECTORY):
        os.makedirs(INPUT_DIRECTORY)
        print(f"Created '{INPUT_DIRECTORY}' folder. Place your .m4a files inside it.")
        
    batch_process_m4a(INPUT_DIRECTORY, OUTPUT_DIRECTORY)
