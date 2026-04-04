m = [1 2 3; 6 5 2; 7 5 3];
[u,v] = eig(m);
r = [1:.2:5];
vol = 4/3*pi*r.^3;
fprintf('done!\n');