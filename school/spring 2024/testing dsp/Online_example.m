clear
clc

Fs = 500;
N = 500;
rng default;
x = ecg(N)'+0.25*randn(N,1);
t = (0:N-1)/Fs;

Fnorm = 75/(Fs/2);
df = designfilt('lowpassfir', 'FilterOrder',70,'CutoffFrequency',Fnorm);

grpdelay(df,2-48,Fs);