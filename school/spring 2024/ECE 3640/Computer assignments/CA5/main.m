clear
clc
close all

[h, Fs] = audioread('Corner bucket basement impulse.wav');

h2 = size(size(h) - 82000);
h2 = h(82000:length(h));

[x, Fsx] = audioread("Nathan-Guitar-chamber.wav");

tic
y1 = myconv(x,h2);
toc

N = nextpow2(length(h2)+length(x)-1);
N = 2^N;

tic 
y2 = fastconv(x,h2,N);
toc
