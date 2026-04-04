clear;clc;
close all;
format compact

x1 = [1;5];x2 = [-5;1];
x1 = x1/norm(x1);x2 = x2/norm(x2);
E = [10, 0;0, 2];P = [x1, x2];

R = P*E*inv(P);

N = 200;
data = mvnrnd([0 0], R, N);

figure(1);scatter(data(:,1), data(:,2));
title('200 Samples from N(0, R)');
xlabel('x_1');ylabel('x_2');
axis equal;grid on;

R_hat = cov(data);
[V, D] = eig(R_hat);[eigenvalues, idx] = sort(diag(D), 'descend');

figure(2);
scatter(data(:,1), data(:,2));hold on;

mean_data = mean(data);
quiver(mean_data(1), mean_data(2), V(1,1),...
    V(2,1), eigenvalues(1)^0.5, 'r', 'LineWidth', 2);
quiver(mean_data(1), mean_data(2), V(1,2),...
    V(2,2), eigenvalues(2)^0.5, 'g', 'LineWidth', 2);

title('Principla Components overlaid on Data');
xlabel('x_1');ylabel('x_2');
axis equal;grid on;
legend('Data points', 'PC1', 'PC2');
