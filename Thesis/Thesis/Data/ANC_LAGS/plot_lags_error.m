clear;
clc;
close all;

%% File settings

filename = 'error_mic.bin';

% Sampling frequency
Fs = 8000;   % Hz

%% Read binary file

fid = fopen(filename, 'rb');

if fid == -1
    error('Could not open file: %s', filename);
end

data = fread(fid, Inf, 'double');

fclose(fid);

%% Create time vector

N = length(data);

t = (0:N-1) / Fs;

%% Plot

figure;

plot(t, data);

xlabel('Time (seconds)');
ylabel('Amplitude');
title(filename);

grid on;

