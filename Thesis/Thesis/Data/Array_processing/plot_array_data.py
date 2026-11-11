import numpy as np
import matplotlib.pyplot as plt
import os

# --------------------------------------------------
# Font sizing notes:
#
# These figures are saved as a single 2x3 grid image and placed at
# (close to) full page width in LaTeX, so there is only ONE scaling
# step between the saved image and the printed page (unlike saving
# 6 separate images that each then get shrunk individually inside a
# subfigure grid). Because of that, these sizes can stay closer to
# "normal" print sizes rather than needing to be inflated to survive
# a second round of shrinking.
# --------------------------------------------------
plt.rcParams.update({
    'font.size': 20,
    'axes.titlesize': 22,     # per-panel titles, e.g. "(-0.5, 1.0)"
    'axes.labelsize': 20,     # shared x/y labels
    'xtick.labelsize': 16,
    'ytick.labelsize': 16,
    'figure.titlesize': 24,   # overall suptitle, if used
})


def read_binary(filename, dtype):
    try:
        with open(filename, 'rb') as file:
            data = np.fromfile(file, dtype=dtype)
    except OSError:
        raise RuntimeError(f"Error opening file: {filename}")

    return data


def load_db_trace(filename, Fs):
    """Read a binary energy file and return (time, amplitude_dB)."""
    error = read_binary(filename, np.float64)
    time = np.arange(len(error)) / Fs
    error_db = 20 * np.log10(
        np.maximum(error, np.finfo(float).tiny)
    )
    return time, error_db


def plot_grid(entries, save_directory, save_name, suptitle, Fs=8000, figsize=(15, 9)):
    """
    Plot 6 traces as a single 2x3 grid figure (one combined image)
    instead of 6 separate images. entries should be a list of
    (filename, panel_title) tuples, ordered left-to-right, top-to-
    bottom (e.g. row 1 = 1.0 m focal points, row 2 = 1.5 m).

    suptitle is the overall figure title (e.g. "Sinusoidal Acoustic
    Source"), shown above the whole 2x3 grid, separate from the
    per-panel coordinate titles.
    """

    fig, axes = plt.subplots(
        2, 3,
        figsize=figsize,
        sharex=True,
        sharey=True,
    )

    for ax, (filename, panel_title) in zip(axes.flat, entries):
        time, error_db = load_db_trace(filename, Fs)

        ax.plot(time, error_db)
        ax.set_title(panel_title)
        ax.set_ylim(-40, 5)
        ax.set_yticks(np.arange(-40, 6, 10))
        ax.grid(True)

    # Overall figure title, above the 2x3 grid.
    fig.suptitle(suptitle)

    # Shared axis labels instead of repeating "Time (s)" / "Amplitude (dB)"
    # on all 6 panels.
    fig.supxlabel("Time (s)")
    fig.supylabel("Amplitude (dB)")

    # rect leaves room at the top for the suptitle so tight_layout
    # doesn't let it overlap the top row of panels.
    fig.tight_layout(rect=[0, 0, 1, 0.94])

    save_path = os.path.join(save_directory, save_name + ".png")
    fig.savefig(save_path, dpi=300, bbox_inches="tight")
    print(f"Saved: {save_path}")

    plt.close(fig)


def calculate_rms(filename, title, Fs=8000, window_duration=0.1):
    """
    Calculate windowed RMS values from an energy.bin file and
    print the minimum RMS, maximum RMS, and corresponding dB values.

    Parameters:
        filename: Path to binary data file
        title: Descriptive name of the experiment
        Fs: Sampling frequency in Hz
        window_duration: RMS window length in seconds
    """

    data = read_binary(filename, np.float64)

    window_size = int(Fs * window_duration)
    num_windows = len(data) // window_size

    # Discard incomplete samples at the end
    data = data[:num_windows * window_size]
    windows = data.reshape(num_windows, window_size)

    rms_values = np.sqrt(np.mean(windows**2, axis=1))
    rms_db = 20 * np.log10(
        np.maximum(rms_values, np.finfo(float).tiny)
    )

    min_rms = np.min(rms_values)
    max_rms = np.max(rms_values)
    min_db = np.min(rms_db)
    max_db = np.max(rms_db)

    print("\n" + "=" * 60)
    print(f"RMS Analysis: {title}")
    print("=" * 60)
    print(f"Window duration:       {window_duration:.3f} s")
    print(f"Window size:           {window_size} samples")
    print(f"Number of windows:     {num_windows}")
    print(f"\nMinimum RMS:           {min_rms:.6f}")
    print(f"Maximum RMS:           {max_rms:.6f}")
    print(f"\nMinimum RMS level:     {min_db:.3f} dB")
    print(f"Maximum RMS level:     {max_db:.3f} dB")
    print(f"\nRMS level variation:   {max_db - min_db:.3f} dB")
    print("=" * 60)


def main():

    Fs = 8000
    save_directory = "plots"
    os.makedirs(save_directory, exist_ok=True)

    # --------------------------------------------------
    # Entries are ordered to match the (a)-(f) panel layout used in
    # the thesis figures: row 1 = 1.0 m focal points (-0.5, 0, 0.5),
    # row 2 = 1.5 m focal points (-0.5, 0, 0.5).
    # --------------------------------------------------

    white_noise_entries = [
        ("microphone_array/white_noise/n5_1_array/energy.bin", "(-0.5, 1.0)"),
        ("microphone_array/white_noise/0_1_array/energy.bin", "(0, 1.0)"),
        ("microphone_array/white_noise/5_1_array/energy.bin", "(0.5, 1.0)"),
        ("microphone_array/white_noise/n5_15_array/energy.bin", "(-0.5, 1.5)"),
        ("microphone_array/white_noise/0_15_array/energy.bin", "(0, 1.5)"),
        ("microphone_array/white_noise/5_15_array/energy.bin", "(0.5, 1.5)"),
    ]

    sinusoidal_entries = [
        ("microphone_array/sinusoidal/n5_1_array/energy.bin", "(-0.5, 1.0)"),
        ("microphone_array/sinusoidal/0_1_array/energy.bin", "(0, 1.0)"),
        ("microphone_array/sinusoidal/5_1_array/energy.bin", "(0.5, 1.0)"),
        ("microphone_array/sinusoidal/n5_15_array/energy.bin", "(-0.5, 1.5)"),
        ("microphone_array/sinusoidal/0_15_array/energy.bin", "(0, 1.5)"),
        ("microphone_array/sinusoidal/5_15_array/energy.bin", "(0.5, 1.5)"),
    ]

    # --------------------------------------------------
    # Generate the two combined grid figures (replaces the previous
    # 12 individually-saved plots)
    # --------------------------------------------------

    plot_grid(
        sinusoidal_entries, save_directory, "sinusoidal_grid",
        suptitle="Sinusoidal Acoustic Source", Fs=Fs,
    )
    plot_grid(
        white_noise_entries, save_directory, "white_noise_grid",
        suptitle="White-Noise Acoustic Source", Fs=Fs,
    )

    # --------------------------------------------------
    # Calculate RMS statistics (unchanged - still per-file)
    # --------------------------------------------------

    print("\n\nRMS RESULTS FOR ALL EXPERIMENTS")

    all_entries = sinusoidal_entries + white_noise_entries
    for filename, title in all_entries:
        calculate_rms(filename, title, Fs)


if __name__ == "__main__":
    main()
