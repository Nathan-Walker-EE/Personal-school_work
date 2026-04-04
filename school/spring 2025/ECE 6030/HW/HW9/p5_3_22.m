clc; clear;
format compact

% Given data
A = [10000, 10001;
     10001, 10002;
     10002, 10003;
     10003, 10004;
     10004, 10005];
b = [20001; 20003; 20005; 20007; 20009];

% (a) Condition numbers
fprintf('(a) Condition Numbers:\n');
cond_A = cond(A);
cond_ATA = cond(A' * A);
fprintf('cond(A) = %.2e\n', cond_A);
fprintf('cond(A^T A) = %.2e\n\n', cond_ATA);

% (b) Pseudoinverse via normal equations
fprintf('(b) Solution using (A^T A)^{-1} A^T b:\n');
x_pseudo = inv(A' * A) * A' * b;
disp('xhat ='); disp(x_pseudo);

% (c) QR Decomposition using Householder (built-in)
fprintf('(c) Solution using QR decomposition:\n');
[Q, R] = qr(A, 0); % Economy-size QR
x_qr = R \ (Q' * b);
disp('xhat ='); disp(x_qr);

% (d) Cholesky factorization (of A^T A)
fprintf('(d) Solution using Cholesky factorization:\n');
AtA = A' * A;
Atb = A' * b;
Rchol = chol(AtA); % Rchol' * Rchol = AtA
y = Rchol' \ Atb;
x_chol = Rchol \ y;
disp('xhat ='); disp(x_chol);

% (e) Comparison
fprintf('(e) Comparison to exact x = [1; 1]:\n');
x_exact = [1; 1];

fprintf('Error from pseudoinverse: %.2e\n', norm(x_pseudo - x_exact));
fprintf('Error from QR:           %.2e\n', norm(x_qr - x_exact));
fprintf('Error from Cholesky:     %.2e\n', norm(x_chol - x_exact));
