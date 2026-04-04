function y = problem2(h)
    
    N = 257

    fid = fopen('../C files/data/cos_10Hz.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*9 , 'double');
    y = y(257:length(y));
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(cos(2*pi * (10/11025) .* n));
    hold off;
    saveas(gcf, 'images/time_convolution_10Hz.png');

    fid = fopen('../C files/data/cos_40Hz.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*9 , 'double');
    y = y(257:length(y));
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(cos(2*pi * (40/11025) .* n));
    hold off;
    saveas(gcf, 'images/time_convolution_40Hz.png');

    fid = fopen('../C files/data/cos_150Hz.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*9 , 'double');
    y = y(257:length(y));
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(cos(2*pi * (150/11025) .* n));
    hold off;
    saveas(gcf, 'images/time_convolution_150Hz.png');

    fid = fopen('../C files/data/cos_350Hz.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*9 , 'double');
    y = y(257:length(y));
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(0*cos(2*pi * (350/11025) .* n));
    hold off;
    saveas(gcf, 'images/time_convolution_350Hz.png');

    fid = fopen('../C files/data/cos_500Hz.bin', 'r');
    if fid == -1
        error('Error opening the file');
    end
    y = fread(fid, N*9 , 'double');
    y = y(257:length(y));
    plot(y);
    hold on;
    L = length(y);
    n = 0:L;
    plot(0*cos(2*pi * (500/11025) .* n));
    hold off;
    saveas(gcf, 'images/time_convolution_500Hz.png');

end