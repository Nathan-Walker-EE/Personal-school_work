clear all; clc; close all; format compact

y1 = @(t) ( exp(-4*(3-t))  + exp(-3*(3-t))  );
y2 = @(t) (  7/12 - 1/4*exp(-4*(3-t)) - 1/3*exp(-3*(3-t)) );

inner_product = @(f, g) integral(@(t) f(t) .* g(t), 0, 3);

I11 = inner_product(y1,y1)
I21 = inner_product(y1,y2)
I12 = inner_product(y2,y1)
I22 = inner_product(y2, y2)

A = [I11, I21;I12, I22];
b = [2;4];

c = A\b

x = @(t) (c(1)*y1(t) + c(2)*y2(t));
check1 = inner_product(x, y1);
check2 = inner_product(x,y2);
fprintf("inner product of x and y1 = %f\n", check1);
fprintf("inner product of x and y2 = %f\n", check2);

t = linspace(0,3,10000);
dt = t(2)-t(1);
figure(1)
plot(t, x(t));
xlabel('Time (t)');
xlim([0 3]);
ylabel('amplitude');
title('x(t)');
grid on;

t = linspace(0, 6, 10000); % Time vector, ensure it covers the expected range
dt = t(2) - t(1); % Time step
x_vals = x(t); % Evaluate x at discrete points
y1_vals = y1(t); % Evaluate y1 at discrete points
y2_vals = y2(t);

Y_vals = conv(x_vals, y1_vals, 'same') * dt; % Convolution approximation
Y_vals = Y_vals/Y_vals(5000) * 2;
t_conv = linspace(0, 6, length(Y_vals)); % Adjusted time vector

figure(2)
plot(t_conv, Y_vals);
xlabel('Time (t)');
xlim([0 3]);
ylabel('Y(t)');
title('Convolution of x(t) with y1(t)');
grid on;

Y2_vals = conv(x_vals, y2_vals, 'same') * dt; % Convolution approximation
Y2_vals = Y2_vals/Y2_vals(5000) * 4;
t_conv = linspace(0, 6, length(Y2_vals)); % Adjusted time vector

figure(3)
plot(t_conv, Y2_vals);
xlabel('Time (t)');
xlim([0 3]);
ylabel('integral of Y(t)');
title('Convolution of x(t) with y2(t)');
grid on;
