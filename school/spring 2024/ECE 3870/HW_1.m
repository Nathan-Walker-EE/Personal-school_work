clear
clc

K = 9e9;
x = -.09;
x2 = .09 - x;
E_force = @(q1,q2,r) ((K*q1*q2)/r^2);

f12 = E_force(-9e-6, 4e-6, x)
f32 = E_force(-36e-6, 4e-6, x2)
f31 = E_force(-36e-6, -9e-6, .09)