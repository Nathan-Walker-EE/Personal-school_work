clear
clc
close all

t = 0:1/100:10-1/100;                     % Time vector
x = cos(pi .* t/8).*cos(pi .* t/2);  

y = fft(x);                               % Compute DFT of x
m = abs(y);                               % Magnitude
y(m<1e-6) = 0;
p = unwrap(angle(y)); 

f = (0:length(y)-1)*100/length(y);        % Frequency vector

subplot(2,1,1)
plot(f,m)
title('Magnitude')


subplot(2,1,2)
plot(f,p*180/pi)
title('Phase')
