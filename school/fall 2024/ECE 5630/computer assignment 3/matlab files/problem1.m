function y = problem1(h)
    N = 512;
    n = (0:N-1)/N - .5; 

    H = fftshift(fft(h,N));
    
    plot(h);
    saveas(gcf, 'images/impulse_response.png');

    plot(n * 11025, abs(H));
    saveas(gcf, 'images/magntitude_response.png');

    plot(n * 11025, rad2deg(angle(H)));
    saveas(gcf, 'images/phase_response.png');

    ideal_H = zeros(1, length(n));
    ideal_H(256-14 : 256+14) = 1;
    plot(n*11025, ideal_H);
    ylim([0 1.5]);
    saveas(gcf, 'images/ideal_response.png');

    y = 1;
end