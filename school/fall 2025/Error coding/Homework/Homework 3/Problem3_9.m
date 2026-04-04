clear all; clc; close all;

P = [0 1 1 1;
     1 1 1 0;
     1 1 0 1;
     1 0 1 1
     ];
G = [eye(4),P];

for i = 0:15
    m = dec2bin(i,4) - '0';
    c = mod(m*G,2);
    disp(['i = ', num2str(i), ' codeword is = ', num2str(c)]);
end