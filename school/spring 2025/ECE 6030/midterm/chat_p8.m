clc; clear; close all;

% Time grid for evaluation
T = 3; % Given time interval [0,3]
N = 3; % Number of basis functions (adjustable)
t = linspace(0, T, 100); % Fine time grid for plotting

% Define impulse response h(t) = exp(-4t) + exp(-3t)
h = @(t) exp(-4*t) + exp(-3*t);

% Generate Legendre basis functions scaled to [0,3]
syms tau;
phi = cell(N,1);
for i = 1:N
    phi{i} = matlabFunction(legendreP(i-1, 2*tau/T - 1)); % Map to [0,3]
end

% Compute response functions psi_i(t) = (h * phi_i)(t)
psi = cell(N,1);
for i = 1:N
    psi{i} = @(t) integral(@(tau) h(tau) .* phi{i}(t - tau), 0, t, 'ArrayValued', true);
end

% Compute Grammian Matrix G
G = zeros(N, N);
for i = 1:N
    for j = 1:N
        G(i,j) = integral(@(tau) psi{i}(tau) .* psi{j}(tau), 0, T, 'ArrayValued', true);
    end
end

% Compute constraint vector b
b = zeros(N, 1);
for i = 1:N
    b(i) = 2 * psi{i}(T) + integral(@(tau) psi{i}(tau), 0, T, 'ArrayValued', true) - 4;
end

% Solve for coefficients c = G \ b
c = G \ b;

% Compute x(t) = sum_i c_i * phi_i(t)
x_t = zeros(size(t));
for i = 1:N
    x_t = x_t + c(i) * phi{i}(t);
end

% Compute output y(t) = (h * x)(t) numerically
y_t = conv(x_t, h(t), 'same') * (t(2) - t(1));

% Plot results
figure;
subplot(2,1,1);
plot(t, x_t, 'b', 'LineWidth', 2);
xlabel('t'); ylabel('x(t)');
title('Optimal Input x(t) with Minimum Energy');
grid on;

subplot(2,1,2);
plot(t, y_t, 'r', 'LineWidth', 2);
xlabel('t'); ylabel('y(t)');
title('System Output y(t)');
grid on;
