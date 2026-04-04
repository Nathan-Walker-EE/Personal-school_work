clear
clc

r = 1e4;
c = 1e-9;

H = tf([0 1/(r*c)], [1 1/(r * c)]);
bode(H)

num = 1/(2 * pi * r * c);
den = 1/(1e3 * j + num);
[mag1, phase1] = bod(num,den);
den = 1/(1e4 * j + num);
[mag2, phase2] = bod(num,den);
den = 1/(5e4 * j + num);
[mag3, phase3] = bod(num,den);

function [mag, phase] = bod(num, den)
    mag = 20*log10(abs(num/den));
    phase = angle(num) - angle(den);
    phase = rad2deg(phase);
end