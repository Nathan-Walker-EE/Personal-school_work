clear all
clc
close all

load lowpass_filter.mat

fileID = fopen('H0.bin', 'r');
% Write the data to the file in double format
x0 = fread(fileID, 'double');
stem(x0);

figure();
fileID = fopen('H1.bin', 'r');
% Write the data to the file in double format
x1 = fread(fileID, 'double');
stem(x1);

figure();
fileID = fopen('H2.bin', 'r');
% Write the data to the file in double format
x2 = fread(fileID, 'double');
stem(x2);

t = zeros(1,479);
for n = 1:159
    t(n*3 - 2) = x0(n);
    t((n*3)-1) = x1(n);
    t((n*3)) = x2(n);
end
figure();
stem(t);
hold on;
stem(h);
hold off;