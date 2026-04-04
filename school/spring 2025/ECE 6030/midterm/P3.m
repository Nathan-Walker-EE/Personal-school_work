clear all
clc
close all

x = [4.8 2.3 2.4 1.8 4.7 1.8 0.6 3.8 2.0, ...
    1.3 2.1 0.4 0.5 4.8 4.8 2.9 0.1 1.2 1.8 4.3];
y = [98.9 21.7 26.1 20.2 98.3 23.3 4.1 75.4 20.7 15.8, ...
    17.6 6.7 1.3 103.3 110.6 48.8 -2.4 17.4 23.1 81.5];
S0 = length(x);
S1 = sum(x);
S2 = sum(x.^2);
S3 = sum(x.^3);
S4 = sum(x.^4);

T0 = sum(y);
T1 = sum(x .* y);
T2 = sum((x.^2) .* y);

A = [S0, S1, S2;
     S1, S2, S3;
     S2, S3, S4];

B = [T0; T1; T2];
coeffs = A\B;

c = coeffs(1);
b = coeffs(2);
a = coeffs(3);

fprintf(['Best-fit quadratic polynomial:' ...
    ' y = %.4fx^2 + %.4fx + %.4f\n'], a, b, c);
scatter(x, y)
xt = 0:.01:5;
yt = a*xt.^2 + b*xt + c;
yhat = [];
for i = 1:length(x)
    inputx = x(i);
    tempy = a*inputx^2 + b*inputx + c;
    yhat = [yhat tempy];
end
e = y - yhat;
fitness = norm(e, 2)^2;
fprintf(['norm squared of error = %f'], fitness)
hold on;
plot(xt,yt);
title('scatter and best fit');
hold off;