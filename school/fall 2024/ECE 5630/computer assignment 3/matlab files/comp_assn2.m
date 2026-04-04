clear all
close all
clc

Fs = 11025;
%this is h in the variables
load lowpass_filter.mat

fileID = fopen('../C files/data/lowpass_filter.bin', 'w');
% Write the data to the file in double format
fwrite(fileID, h, 'double');

% y = problem1(h);
% y = problem2(h);
% y = problem3(h);
y = problem4(h);
