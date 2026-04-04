clear all; clc;close all;

Fs = 8000;
n = 0:Fs*3;
input = sin(2*pi*.05.*n);
g = 2;
output = distort_audio(input, g);

figure(1)
plot(n/Fs, input);
hold on;
plot(n/Fs, output);
title('input and output of distortion')
xlabel('amplitude')
ylabel('time')
legend('input', 'output')
xlim([.35 .45]);