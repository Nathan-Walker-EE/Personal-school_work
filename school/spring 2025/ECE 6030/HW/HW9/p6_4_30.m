clc;clear;
format compact

R11 = [1;2;1];
R12 = [-2;1;0];
R21 = [-1;-2;5];

P = [R11, R21, R12]
E = eye(3);
E(3,3) = 2;
A = P * E * inv(P)
e = eig(A)

x1 = [1;2;3] * 1/sqrt(14);
x2 = [-2;1;0] * 1/sqrt(5);
x3 = [-3;-6;5] * 1/sqrt(70);
P = [x1, x2, x3];
E = eye(3);
E(2,2) = 4;
E(3,3) = 9
A = P * E * inv(P)
e = eig(P*E*inv(P))