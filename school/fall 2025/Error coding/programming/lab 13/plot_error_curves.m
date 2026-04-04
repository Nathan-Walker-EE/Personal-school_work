clc; close all; clear all; format compact;

% -----------------------------
% Data
% -----------------------------
R_12_err = [0.17175, 0.13634, 0.118007, 0.10386, 0.0876725];
R_12_SNR = [-1, 0, .25, .5, .75];

R_13_err = [0.0872993, 0.0726233, 0.048422, 0.00979333, 0.0014967,...
            0.0003571, 1.468e-5];
R_13_SNR = [0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1];

R_gall_err = [0.026849, 0.014449, 0.009749, 0.002949, 0.001949, 0.000299,...
              0.0004, 0.00059];
R_gall_SNR = [4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5];

% -----------------------------
% Figure 1 — R = 1/2
% -----------------------------
figure;
semilogy(R_12_SNR, R_12_err, 'o-', 'LineWidth', 1.6, 'MarkerSize', 8);
grid on;
xlabel('SNR (dB)', 'FontSize', 12);
ylabel('Bit Error Rate (BER)', 'FontSize', 12);
title('BER vs SNR for R = 1/2 Code', 'FontSize', 14);
ylim([1e-3 1]);   % adjust if desired

% -----------------------------
% Figure 2 — R = 1/3
% -----------------------------
figure;
semilogy(R_13_SNR, R_13_err, 's-', 'LineWidth', 1.6, 'MarkerSize', 8);
grid on;
xlabel('SNR (dB)', 'FontSize', 12);
ylabel('Bit Error Rate (BER)', 'FontSize', 12);
title('BER vs SNR for R = 1/3 Code', 'FontSize', 14);
ylim([1e-6 1]);   % adjust if desired

% -----------------------------
% Figure 3 — Gallager Code
% -----------------------------
figure;
semilogy(R_gall_SNR, R_gall_err, 'd-', 'LineWidth', 1.6, 'MarkerSize', 8);
grid on;
xlabel('SNR (dB)', 'FontSize', 12);
ylabel('Bit Error Rate (BER)', 'FontSize', 12);
title('BER vs SNR for Gallager Code', 'FontSize', 14);
ylim([1e-4 1]);   % adjust if desired
