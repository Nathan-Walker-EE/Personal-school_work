close all
clc
clear all

% Parameters
fs = 1000;             % Sampling frequency in Hz
f_signal = 50;         % Frequency of the input signal (Hz)
f_carrier = 200;       % Carrier frequency (Hz)
num_samples = 1000;    % Number of samples to process

% Initialize phase accumulator
phase = 0;                    
phase_increment = 2 * pi * f_carrier / fs;  % Phase increment per sample

% Preallocate arrays for efficiency
f = zeros(1, num_samples);          % Input signal
carrier_samples = zeros(1, num_samples);  % Carrier samples
modulated = zeros(1, num_samples);  % Modulated signal

% Sample-by-sample processing loop
for n = 1:num_samples
    % Generate input signal (sinusoidal signal)
    f(n) = sin(2 * pi * f_signal * (n - 1) / fs);
    
    % Generate carrier sample using the current phase
    carrier_samples(n) = cos(phase);
    
    % Modulate the input signal with the carrier
    modulated(n) = f(n) * carrier_samples(n);
    
    % Update the phase accumulator and wrap it within [0, 2*pi]
    phase = phase + phase_increment;
    if phase > 2 * pi
        phase = phase - 2 * pi;
    end
end

% Plot the signals
t = (0:num_samples-1) / fs;  % Time vector

figure;
subplot(3,1,1);
plot(t, f);
title('Input Signal f[n]');
xlabel('Time (s)');
ylabel('Amplitude');

subplot(3,1,2);
plot(t, carrier_samples);
title('Carrier Signal cos(2\pi f_{carrier} n)');
xlabel('Time (s)');
ylabel('Amplitude');

subplot(3,1,3);
plot(t, modulated);
title('Double-Sideband Modulated Signal');
xlabel('Time (s)');
ylabel('Amplitude');
