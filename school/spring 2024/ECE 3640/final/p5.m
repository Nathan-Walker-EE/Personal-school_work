clear
clc
close all

n = 0:100;
N = 101;
NFFT = 2^15;
f = (0 : NFFT-1)/NFFT - 0.5;

x = exp(j * 2 * pi * 0.1 .* n);
x2 = exp(j * 2 * pi * .02 .* n);
A = x .* exp(j * 2 * pi * .02 .* n);
plot(f,abs(fftshift(fft(A,NFFT))));
B = upsample(A,3);

h = load('p5_lowpass.fcf').';

C = conv(B, h);
n = 0:916;
D = C .* exp(j * 2 * pi * .02 .* n);
out = downsample(D,2);

plot(f,abs(fftshift(fft(out,NFFT))));

