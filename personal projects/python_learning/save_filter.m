clear all
clc
close all

load filter.mat

fileID = fopen('filter.bin', 'w');
% Write the data to the file in double format
fwrite(fileID, h, 'double');