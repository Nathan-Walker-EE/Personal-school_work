function y = plot_h(h, save)
    figure();
    plot(h);
    if save == 1
        saveas(gcf, 'images/impulse_response.png', 'png');
    end
    
    N = 512; 
    n = (0:N-1)/N -.5;
    
    figure();
    plot(n, abs(fftshift(fft(h,N))));
    if save == 1
        saveas(gcf, 'images/magnitude_H.png', 'png');
    end
    figure();
    plot(n, rad2deg(angle(fftshift(fft(h,N)))));
    if save == 1
        saveas(gcf, 'images/Phase_H.png', 'png');
    end
    y = h;
end

