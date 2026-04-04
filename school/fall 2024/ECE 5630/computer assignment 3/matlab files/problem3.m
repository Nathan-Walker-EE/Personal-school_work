function y = problem3(h)
    N = 1024; % Number of complex numbers (adjust as needed)

    fid = fopen('../C files/data/cos_10Hz_fft.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*2 , 'float');
    figure();
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(cos(2*pi * (10/11025) .* n));
    hold off;
    saveas(gcf, 'images/fast_convolution_10Hz.png');

    fid = fopen('../C files/data/cos_40Hz_fft.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*2 , 'float');
    figure();
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(cos(2*pi * (40/11025) .* n));
    hold off;
    saveas(gcf, 'images/fast_convolution_40Hz.png');

    fid = fopen('../C files/data/cos_150Hz_fft.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*2 , 'float');
    figure();
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(cos(2*pi * (150/11025) .* n));
    hold off;
    saveas(gcf, 'images/fast_convolution_150Hz.png');

    fid = fopen('../C files/data/cos_350Hz_fft.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*2 , 'float');
    figure();
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(0 * cos(2*pi * (350/11025) .* n));
    hold off;
    saveas(gcf, 'images/fast_convolution_350Hz.png');

    fid = fopen('../C files/data/cos_500Hz_fft.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*2 , 'float');
    figure();
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(0 * cos(2*pi * (500/11025) .* n));
    hold off;
    saveas(gcf, 'images/fast_convolution_500Hz.png');

end