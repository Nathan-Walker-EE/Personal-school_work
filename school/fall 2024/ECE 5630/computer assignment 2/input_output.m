clear all 
clc
close all

% fileID = fopen('input_cos.bin', 'r');
% Write the data to the file in double format
% x = fread(fileID, 'double');

n = 1:47900;
f = 1/16;
x16 = cos(2 * pi * f .* n);
x8 = cos (2 * pi * (1/8) .* n);
x4 = cos(2 * pi * (1/4) .* n);

fileID = fopen('outputM_cos_8_p5.bin', 'r');
% Write the data to the file in double format
y = fread(fileID, 'double');

N = 512;
n = (0:N-1)/N -.5;
X = fftshift(fft(x,N));
Y = fftshift(fft(y,N));

figure();
plot(n, abs(X));
hold on;
plot(n, abs(Y));
hold off;

