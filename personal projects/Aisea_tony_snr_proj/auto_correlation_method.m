clear all
clc

N = 1024*2;
n = 0:N-1;
f = .25;
w = 2*pi*f;

% x = sin(w .* n);
[x, Fs] = audioread('12acguit - 12 String D4..wav');
X = fft(x);
Xmag = abs(X).^2;
rxfft = ifft(Xmag)/N;
rxconv = xcorr(x, 'unbiased');

figure(1);
plot(rxfft);
hold on;
plot(rxconv);
hold off;
legend('fft method', 'convolution method');

figure(2);
plot(x);
hold on;
plot(rxconv);
hold off;
legend('x', 'rxconv');

figure(3);
plot(x);
hold on;
plot(rxfft);
hold off;
legend('x', 'rxfft');