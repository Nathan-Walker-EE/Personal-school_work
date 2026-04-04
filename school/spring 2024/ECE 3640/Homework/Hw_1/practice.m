clear
clc

F = [-5:.01:5];
T = 1;
x = T * sinc(F .* T);

plot_mag(x,F,1);