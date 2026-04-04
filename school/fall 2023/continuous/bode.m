clc
clear

syms s

A = (4 + s)*(20 + s);
num = [.5 12500];
den = [1 12500];

TF = tf(num,den)
bode(TF);