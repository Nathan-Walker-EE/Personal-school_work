clear all;clc; close all;

N = 1000;
s1 = 2*rand(1,N)-1;
s2 = 2*rand(1,N)-1;
figure();
scatter(s1,s2, '.');
axis([-5 5 -5 5]);
axis("equal");

A = [2.5 2; 1 3];
y = A*[s1;s2];
x1 = y(1,:);
x2 = y(2,:);
figure();
scatter(x1,x2,'.');
axis([-5 5 -5 5]);
axis("equal")

new_y = [];
w0 = eye(2);
w = w0;
N_iterations = 4;
y_iter = y;
figure();
for i = 1:N_iterations
    [y_temp, w] = bss(y, w0, .001, 1);
    w0 = w;
    new_y = y_temp;
    x1 = new_y(1,:);
    x2 = new_y(2,:);
    scatter(x1,x2,'.');
    hold on;
    axis([-5 5 -5 5]);
    axis("equal")
end
legend('1st iteration', '2nd iteration', '3rd iteration', '4th iteration');

load x1
load x2 
load x3
x = [x1;x2;x3];

fprintf('separating the data ...\n');
W0 = eye(3);
mu = 0.00025;
Niter = 5;
xiter = x;
for i = 1:Niter
    [y, W] = bss(xiter, W0, mu,2);
    W0 = W;
end
% soundsc(y(3,:));

