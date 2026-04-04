clear all
clc
format compact

load doadata1.dat

s1 = doadata1(:,1) + j*doadata1(:,2);
s2 = doadata1(:,3) + j*doadata1(:,4);
x = [s1.';s2.'];

R = (x * x');
[V, D] = eig(R);

eigenvalues = diag(D);
[~,idx] = max(eigenvalues);
vmax = V(:,idx);
vmax = vmax * (1/0.707);
direction = rad2deg(angle(vmax(1)));
display(['angle of arrival of signal ~= ', num2str(direction)])
