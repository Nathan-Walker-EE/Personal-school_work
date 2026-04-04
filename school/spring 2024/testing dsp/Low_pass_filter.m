clear
clc

[y,Fs] = audioread("fireflyintro.wav");


length(y)/Fs

for n = 4:length(y)
    y(n) =  y(n)*1/(y(n-1)) * 1/6;
end

Y = fft(y);

t = 0:1/Fs:length(y)/Fs;
noise = sin(2 * pi * 400 * t);
x = randn(size(y))/4;
X = x + y;
sound(y,Fs);


f = (0:length(y)-1)*Fs/length(y);
plot(f,abs(y))
xlabel('Frequency (Hz)')
ylabel('Magnitude')
title('Magnitude')

Y = fft(y);
n = length(y);
fshift = (-n/2:n/2-1)*(Fs/n);
yshift = fftshift(Y);
plot(fshift,abs(yshift))
