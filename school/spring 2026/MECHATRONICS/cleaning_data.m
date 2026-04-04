clear all; clc; close all; format compact;

%% grabs data from .csv file and puts it in variables
tab = readtable("flight_data.csv");
tab = table2array(tab);
true_accel_data = tab(2:end, 5:7);
x_data = true_accel_data(:,1);
y_data = true_accel_data(:,2);
z_data = true_accel_data(:,3);

%% plots graph of true data for robustness
plot_graphs = 0;
if plot_graphs == 1 
    figure();
    plot((0:(length(true_accel_data)-1)), x_data);
    figure();
    plot((0:length(true_accel_data)-1), y_data);
    figure();
    plot((0:length(true_accel_data)-1), z_data);
end

%% make observations of the true data 
%corrupts by adding noise
%need to come up with a better A matrix than eye(3)
y = [];
for i = 1:length(true_accel_data)
    noise = randn(3,1)*5;
    y = [y ,true_accel_data(i, 1:3).' + noise;];
end
y = y.';

%% plot obersvations
if plot_graphs == 1
    figure();
    plot((0:length(true_accel_data)-1)/50, y(:,1));
    figure();
    plot((0:length(true_accel_data)-1)/50, y(:,2));
    figure();
    plot((0:length(true_accel_data)-1)/50, y(:,3));
end

%% Attempt to clean data via Kalman
xhat = true_accel_data(1,1);
P = 1;

A = 1;
C = 1;
sigma_meas = std(true_accel_data(1:length(true_accel_data), 1));
R = sigma_meas^2;
Q = 250;
xest = [xhat];
P_est = [P];
for k = 2:length(true_accel_data)
    y = true_accel_data(k,1);
    [xhat, P] = kalman(y, xhat, P, A, C, Q, R);
    P_est = [P_est P];
    xest = [xest, xhat];
end

plot(x_data);
hold on;
plot(xest, 'LineWidth', 1);

c_data = load('estimated_state.txt');
plot(c_data, 'lineWidth', 1);
hold off;
legend(["accel data", "Matlab", "C data"]);

figure();
plot(P_est);
hold on;
P_data = load('estimated_P.txt');
plot(P_data);
hold off;
legend(["matlab"], ["C program"]);

%% different ratios
xhat = true_accel_data(1,1);
P = 1;

A = 1;
C = 1;
sigma_meas = std(true_accel_data(1:length(true_accel_data), 1));
R = sigma_meas^2;
Q = 250*5;
Q*10
xest = [xhat];
P_est = [P];
xest_mat = zeros(1,length(true_accel_data));
for l = 1:4
for k = 2:length(true_accel_data)
    y = true_accel_data(k,1);
    [xhat, P] = kalman(y, xhat, P, A, C, Q, R);
    P_est = [P_est P];
    xest = [xest, xhat];
end
Q = Q/10;
xest_mat = [xest_mat;xest];
xest = [true_accel_data(1,1)];
end

figure(1);
plot(x_data);
hold on;
plot(xest_mat(2,:), 'LineWidth', 1);
plot(xest_mat(3,:), 'LineWidth', 1);
plot(xest_mat(4,:), 'LineWidth', 1);
plot(xest_mat(5,:), 'LineWidth', 1);
legend('unfiltered','Q/R = 1','Q/R = .1', 'Q/R = 0.01', 'Q/R = 0.001')

