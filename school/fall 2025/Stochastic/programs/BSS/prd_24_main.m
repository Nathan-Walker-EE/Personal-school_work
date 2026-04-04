clear all; close all; clc; format compact;

load prog1dat.mat

X1 = X(1:2,:);
mu_hat11 = mean(X1(1,:));
mu_hat12 = mean(X1(2,:));
mu_hat1 = [mu_hat11, mu_hat12];
cov_11 = cov(X1');
X2 = X(3:4,:);
mu_hat21 = mean(X2(1,:));
mu_hat22 = mean(X2(2,:));
mu_hat2 = [mu_hat21, mu_hat22]
cov_22 = cov(X2')
cov_21 = cov([X1', X2'])
plot(X1(1,:),X1(2,:),'.','MarkerSize',10);

SIGMA = cov([X(1,:)', X(2,:)', X(3,:)', X(4,:)'])
Mu = mean(X')'
[x4hat, sigma4squared] = predictx4(3, 3.6, 5.2, SIGMA, Mu)
[xhat, Sigma22] = predictx24(5, 7, SIGMA, Mu)