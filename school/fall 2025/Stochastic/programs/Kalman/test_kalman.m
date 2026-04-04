clear all; clc; close all;format compact

h = 0.001;
m = 1;
Atild = [0,1;0,0];
btild = [0;1/m];
xt = zeros(2,1);
xtt = zeros(2,1);

A12 = eye(4);A12(1,2) = h;A12(3,4)=h;
B = zeros(4,2);B(2,1)=h/m;B(4,2)=h/m;
% w = B*f, f=input
w = eye(4);w(1,1)=2;w(1,2)=1;w(2,1)=1;w(2,2)=2;w(3,3)=3;w(3,4)=2;w(4,3)=2;
w(4,4)=3;
v = eye(2);

% simulation

xt = [0;0;0;0];P = w;xhat = xt;
Lw = chol(w,'lower');
Lv = chol(v,'lower');
ymat = [1,0,0,0;0,0,1,0];
xtrue = [];xest = [];
for i = 1:5000
    wt = randn(4,1);
    xt = A12*xt + (Lw*wt);
    vt = randn(2,1);
    yt = ymat*xt + (Lv*vt);
    [xhat,P] = kalman(yt,xhat,P,A12,ymat,w,v);
    xtrue=[xtrue,xt];xest=[xest,xhat];
end
sprintf("finished\n")
display(xhat);
display(xt)

figure(1);
plot(xest(1,:),xest(3,:));
hold on;
plot(xtrue(1,:),xtrue(3,:));
hold off;
legend('estimate', 'true');
title('position');

figure(2);
plot(xest(2,:),xest(4,:));
hold on;
plot(xtrue(2,:),xtrue(4,:));
hold off;
legend('estimate', 'true');
title('velocity');

