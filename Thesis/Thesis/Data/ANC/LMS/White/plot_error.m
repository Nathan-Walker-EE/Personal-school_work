clear;
clc;
close all;

%% Data path

data_path = "data/thesis_data/ANC/LMS/White/";

%% Read experiment 001

fid = fopen(data_path + "adaptive_filter_001.bin", "rb");
filter_001 = fread(fid, Inf, "double");
fclose(fid);

fid = fopen(data_path + "error_mu_001.bin", "rb");
error_001 = fread(fid, Inf, "double");
fclose(fid);

%% Read experiment 0001

fid = fopen(data_path + "adaptive_filter_0001.bin", "rb");
filter_0001 = fread(fid, Inf, "double");
fclose(fid);

fid = fopen(data_path + "error_mu_0001.bin", "rb");
error_0001 = fread(fid, Inf, "double");
fclose(fid);

%% Read experiment 00001

fid = fopen(data_path + "adaptive_filter_00001.bin", "rb");
filter_00001 = fread(fid, Inf, "double");
fclose(fid);

fid = fopen(data_path + "error_mu_00001.bin", "rb");
error_00001 = fread(fid, Inf, "double");
fclose(fid);

%% Create figure

figure;

%% Adaptive filter

subplot(2,1,1);

plot(filter_001, 'DisplayName', '001');
hold on;
plot(filter_0001, 'DisplayName', '0001');
plot(filter_00001, 'DisplayName', '00001');

xlabel('Tap');
ylabel('Coefficient');
title('Adaptive Filter');
legend;
grid on;

%% Error

subplot(2,1,2);

plot(error_001, 'DisplayName', '001');
hold on;
plot(error_0001, 'DisplayName', '0001');
plot(error_00001, 'DisplayName', '00001');

xlabel('Sample');
ylabel('Error');
title('Error');
legend;
grid on;

%% Overall title

sgtitle('LMS Experiments');