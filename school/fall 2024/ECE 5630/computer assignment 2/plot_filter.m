clear all
clc
close all

% wp = .12
% ws = .13
% Ap = .001
% As = 80
load lowpass_filter.mat
fileID = fopen('lowpass_filter.bin', 'w');

% Write the data to the file in double format
fwrite(fileID, h, 'double');

plot(h);
xlabel('samples');
ylabel('amplitude');
title('impulse response');
saveas(gcf, "impulse_response.png");

N = 2^14;
n = (0:N-1)/N;
H = fft(h, N);

plot(n, 20*log10(abs(H)));
xlabel('cycles/sample');
ylabel('magnitude');
title('Magnitude of H');
saveas(gcf, "Magnitude_dB.png");

plot(n, rad2deg(angle(H)))
xlabel('cycles/sample');
ylabel('angle (degrees)');
title('Phase Response of H');
saveas(gcf, "Phase_response.png");
