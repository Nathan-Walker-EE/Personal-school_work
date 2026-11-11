close all
clc
clear all

type = 'double';
N = 2^16;
n = (0:N-1)/N - .5;
Fs = 8000;


%% plot n5_15 microphone array sinusoid
% this data is the source inside the aperature moving out of
% the aperature showing its difference
error = read_binary('microphone_array/sinusoidal/n5_15_array/energy.bin', type);
time = 0:length(error)-1;
time = time/Fs;
figure(1);
plot(time,error);
figure(2);
plot(time,20*log10(error));

%% plot 5_15 microphone array sinusoid
% this data is the source outside of the aperature moving into
% the aperature showing its difference
error = read_binary('microphone_array/sinusoidal/5_15_array/energy.bin', type);
time = 0:length(error)-1;
time = time/Fs;
figure(1);
plot(time,error);
figure(2);
plot(time,20*log10(error));

%% plot n5_1 microphone array sinusoid
%  random movment about the focal point
error = read_binary('microphone_array/sinusoidal/n5_1_array/energy.bin', type);
time = 0:length(error)-1;
time = time/Fs;
figure(1);
plot(time,error);
figure(2);
plot(time,20*log10(error));

%% plot 5_1 microphone array sinusoid
%  random movment about the focal point
error = read_binary('microphone_array/sinusoidal/5_1_array/energy.bin', type);
time = 0:length(error)-1;
time = time/Fs;
figure(1);
plot(time,error);
figure(2);
plot(time,20*log10(error));

%% plot 0_15 microphone array sinusoid
%  random movment about the focal point
error = read_binary('microphone_array/sinusoidal/0_15_array/energy.bin', type);
time = 0:length(error)-1;
time = time/Fs;
figure(1);
plot(time,error);
figure(2);
plot(time,20*log10(error));

%% plot 0_1 microphone array sinusoid
%  random movment about the focal point
error = read_binary('microphone_array/sinusoidal/0_1_array/energy.bin', type);
time = 0:length(error)-1;
time = time/Fs;
figure(1);
plot(time,error);
figure(2);v
plot(time,20*log10(error));
