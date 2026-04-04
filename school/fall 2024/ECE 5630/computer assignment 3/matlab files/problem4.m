function y = problem4(h)
    
    fid = fopen('../C files/data/audio_out_fft.bin', 'r');
        if fid == -1
            error('Error opening the file');
        end
    
    y = fread(fid, 496125, 'float');
    y = y/max(y);
    audiowrite("wav files/audio_out_fft.wav", y, 11025);

    fid = fopen('../C files/data/audio_out.bin', 'r');
        if fid == -1
            error('Error opening the file');
        end
    
    y = fread(fid, 496125, 'float');
    y = y/max(y);
    audiowrite("wav files/audio_out_time.wav", y, 11025);
end