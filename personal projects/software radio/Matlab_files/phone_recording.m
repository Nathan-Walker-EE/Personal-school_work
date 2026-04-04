clear all
close all
clc

[loseless, Fsls] = audioread("Lossless.m4a");
[compressed, Fscm] = audioread("Compression.m4a");

N = 2^16;
n = ((0:N - 1)/N - .5);
L = fft(loseless, N);
N1 = length(L);
n1 = ((0:N1-1)/N1 - .5) * Fsls;
plot(n * Fsls, abs(fftshift(L)));
title("loseless");

figure();
C = fft(compressed, N);
N2 = length(C);
n2 = ((0:N2-1)/N2 - .5) * Fscm;
plot(n * Fscm, abs(fftshift(C)));
title("compressed");