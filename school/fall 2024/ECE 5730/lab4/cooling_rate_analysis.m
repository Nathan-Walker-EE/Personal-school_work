close all
clc
clear all

% Read the data from CSV file
data = readmatrix('simulated_annealing_results.csv');

% Extract columns for cooling rate, energy, and time
coolingRates = data(:,1);
energy = data(:,2);
time = data(:,3);

% Plot energy vs cooling rate
figure;
plot(coolingRates, energy, '-o');
xlabel('Cooling Rate');
ylabel('Energy (Solution Quality)');
title('Energy vs Cooling Rate');

% Plot time vs cooling rate
figure;
plot(coolingRates, time, '-o');
xlabel('Cooling Rate');
ylabel('Time (ms)');
title('Time vs Cooling Rate');