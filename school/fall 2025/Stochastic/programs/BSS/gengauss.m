function X = gengauss(mu, Sigma, N)
    z = randn(length(mu),N);
    L = chol(Sigma, 'lower');
    X = L*z;
    X = X + mu;
end