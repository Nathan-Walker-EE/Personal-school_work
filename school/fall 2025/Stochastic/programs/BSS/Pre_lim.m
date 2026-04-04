clear all; clc; close all; format compact

%% exercise 1
n = 1000;
X = randn(1,n);
X = X*sqrt(5) + 2;
histogram(X, 'Normalization', 'probability');
t = -5:.01:10;
gaus = 1/(sqrt(2*pi*5));
gaus = gaus * exp(-(t - 2).^2/(2*5));
hold on;
plot(t,gaus);
hold off;
legend('randn','pdf');
xlabel('X=x');
ylabel('P(X=x)');
title('Gaussian:Histogram vs pdf');

%% exercise 2
n = 1000;
b = 5;
a = -2;
X = rand(1,n);
X =  X*(b-a) + a;
histogram(X, 'Normalization', 'probability');
t = -2:0.01:5;
uniform = 1/(b-a);
uniform = ones(size(t))*uniform;
hold on;
plot(t,uniform);
hold off;
legend('rand', 'pdf');
xlabel('X=x');
ylabel('P(X=x)');
title('Uniform:Histogram vs pdf')

%% exercise 3

mu = [1;2];
sigma = [1, 1;1, 3];
N = 1000;
X = gengauss(mu,sigma, N);
mu_hat1 = mean(X(1,:))
mu_hat2 = mean(X(2,:))
cov_hat = cov(X')
% var_hat1 = var(X(1,:))
% var_hat2 = var(X(2,:))
plot(X(1,:), X(2,:), '.','MarkerSize', 1);
axis('equal');
hold on;
[u,v] = eig(inv(sigma));
v1 = u(:,1)/sqrt(v(1,1));
v2 = u(:,2)/sqrt(v(2,2));
plot([mu(1),mu(1)+v1(1)], [mu(2), mu(2) + v1(2)], 'r');
plot([mu(1),mu(1)+v2(1)], [mu(2), mu(2) + v2(2)], 'r');

c_values = [2.30, 6.18, 11.83];  % ≈ 1σ, 2σ, 3σ
colors = ['r', 'm', 'b'];        % colors for each ellipse

A = inv(sigma);
x0 = mu;

for i = 1:length(c_values)
    c = c_values(i);
    x_ellipse = plotellipse(A, x0, c);
    plot(x_ellipse(1,:), x_ellipse(2,:), colors(i), 'LineWidth', 2);
end

legend('Samples', 'Principal Axes', '', '1σ', '2σ', '3σ');
title('Gaussian samples with confidence ellipses');
xlabel('X1');
ylabel('X2');
hold off;shg;