clear; clc; close all;format compact
g = [1, 1, 0, 1];
c = [];
for i = 0:15
    binary = dec2bin(i, 4);
    x = [str2num(binary(4)),str2num(binary(3)),...
        str2num(binary(2)),str2num(binary(1)),];
    temp = fliplr(conv(x,g));
    temp = mod(temp, 2);
    c = [c;temp];
    disp(poly2sym(temp))
end
parity = [1 0 0 0 0 0 0 1];h = [1 0 1 1 1];
[Q1, R1] = deconv(mod(conv(h,c(4,:)),2),parity)
[Q2, R2] = deconv(mod(conv(h,c(11,:)),2),parity)
[Q3, R3] = deconv(mod(conv(h,c(15,:)),2),parity)