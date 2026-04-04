function distorted_signal = distort_audio(input_audio, g)
    distorted_signal = tanh(input_audio * g);
end