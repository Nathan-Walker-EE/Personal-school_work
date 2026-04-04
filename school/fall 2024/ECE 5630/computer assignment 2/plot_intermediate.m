clear all
clc
close all

fileID = fopen('H0_output.bin', 'r');
% Write the data to the file in double format
x = fread(fileID, 'double');
stem(x); hold on;
fileID = fopen('H1_output.bin', 'r');
% Write the data to the file in double format
x = fread(fileID, 'double');
stem(x); 
fileID = fopen('H2_output.bin', 'r');
% Write the data to the file in double format
x = fread(fileID, 'double');
stem(x); 

fileID = fopen('outputM_cos_16.bin', 'r');
% Write the data to the file in double format
y = fread(fileID, 'double');
stem(y);
xlim([4000 4030]);
legend('H0', 'H1', 'H2', 'test');
hold off;

figure(); stem(y); xlim([4000 4030]);