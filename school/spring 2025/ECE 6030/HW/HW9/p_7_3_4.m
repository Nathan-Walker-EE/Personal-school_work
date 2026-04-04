A = [1 4 5 6; 6 7 2 1];
b = [48; 30];

[U,S,V] = svd(A, 'econ');         % Compute reduced SVD: A = U*S*V'
S_inv = diag(1 ./ diag(S));       % Invert singular values
x_ls = V * S_inv * U' * b;       % Compute minimum-norm least squares solution

x = [1;2;3;4];
bhat = A*x_ls;
xnorm = norm(x,2);
xlsnorm = norm(x_ls,2);
disp('xls = ');disp(x_ls)
disp('bhat =');disp(bhat)
disp('||x|| =');disp(xnorm)
disp('||xls|| =');disp(xlsnorm)
