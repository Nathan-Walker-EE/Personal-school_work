clear all
clc
close all

data = importdata('firfilt32.dat');
data = data(2:end,1);
a = [1 data.'];
N = length(a);

% Initialize variables
reflection_coeffs = zeros(1, N);  % Store k1, k2, ..., kN
a_curr = a;  % Current filter coefficients

reflection_coeffs(N) = a_curr(N);
for m = N:-1:1
    reflection_coeffs(m) = a_curr(m);
    a_next = zeros(1,m-1);
    a_next(1) = 1;
    for k = 2:m
        a_next(k) = a_curr(k) - a_curr(m) * a_curr(m - k + 1);
        a_next(k) = a_next(k)/(1-a_curr(m)^2);
    end
    a_curr = a_next;
end
format compact
reflection_coeffs
true = tf2latc(a).'
