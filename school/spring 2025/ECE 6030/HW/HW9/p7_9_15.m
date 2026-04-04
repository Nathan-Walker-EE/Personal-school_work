A = [4.84726, -2.46789, 0.549036, -4.31815;
     0.813505, -3.46221, -0.251525, 2.97353];
B = [4.51076, -2.18889, 0.58951, -4.715;
     1.95213, -3.64499, -0.131083, 2.2982];

C = A * B.';
% SVD
[U, S, V] = svd(C);

% Compute rotation
R = U * V'; Q = R*B

error = norm(A - Q, 2);

% Output
format compact
disp('A ='); disp(A);disp('B*R ='); disp(Q)
disp('Error ='); disp(error)
disp('rotation ='); disp(rad2deg(atan(Q(2,1)/Q(1, 1))))
