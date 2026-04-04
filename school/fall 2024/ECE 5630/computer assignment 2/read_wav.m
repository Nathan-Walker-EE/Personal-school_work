clear all
clc
close all

Fs2 = 8269;
[x, Fs] = audioread('ghostbustersray.wav');

fileID = fopen('problem6_poly.bin', 'r');

% Write the data to the file in double format
y = fread(fileID, 'double');
y = y/max(y);
audiowrite('problem6_poly.wav', y, Fs2);

N = 2^14;
n = (0:N-1)/N -.5;
X = fftshift(fft(x,N));
Y = fftshift(fft(y,N));

figure();
plot(n, abs(X));
hold on;
plot(n, abs(Y));
hold off;
legend('x', 'y')

% sound(x,Fs);
sound(y,Fs2);

