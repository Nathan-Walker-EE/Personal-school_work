function y = plot_dft(freq, filename, n, ofilename, plottitle)

    x = cos(2 * pi * freq .* n);
    fileID = fopen(filename, 'r');
    y = fread(fileID, 'double');

    N = 512;
    n2 = (0:N-1)/N -.5;
    X = fftshift(fft(x,N));
    Y = fftshift(fft(y,N));
    
    figure();
    plot(n2, abs(X));
    hold on;
    plot(n2, abs(Y));
    hold off;
    title(plottitle);
    legend('X', 'Y');
    saveas(gcf, ofilename)
end