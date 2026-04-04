clear all
close all
clc

[x, Fs] = audioread("fireflyintro.wav");
x = resample(x, 5, 2);
Fs = Fs * 5;

X = fftshift(fft(x));
nx = (0:length(X)-1)/length(X) - .5;
plot(nx, abs(X));

n = 0:99;
phase = 0;
phase_inc = 2 * pi * .15;
y = zeros(1,length(x));
for n = 1:length(x)
    y(n) = x(n) * cos(phase);
    phase = phase + phase_inc;
    if phase > 2*pi
       phase = phase - 2 * pi;
    end
end

Y = fftshift(fft(y));
nx = (0:length(Y)-1)/length(Y) - .5;
figure();
plot(nx, abs(Y));

