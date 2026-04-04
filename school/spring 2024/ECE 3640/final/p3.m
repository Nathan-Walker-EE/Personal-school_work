clear
clc
close all

x = generate_signal();

N = length(x);

win1 = ones(1,N);
win2 = hamming(N).';
win3 = blackman(N).';
h1 = x .* win1;
h2 = x .* win2;
h3 = x .* win3;

figure
plot(0:N-1, h1);
figure
plot(0:N-1, h2);
figure
plot(0:N-1, h3);

NFFT = 2^14;
H1 = fft(h1, NFFT);
H2 = fft(h2, NFFT);
H3 = fft(h3, NFFT);

f = (0:NFFT-1)/NFFT - 0.5;
figure
plot(f, 20*log10(abs(fftshift(H1))));
title('rectangle window')
figure
plot(f, 20*log10(abs(fftshift(H2))));
title('hamming')
figure
plot(f, 20*log10(abs(fftshift(H3))));
title('blackman')