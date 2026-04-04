function [x4hat,sigma_4squared] = predictx4(x1, x2, x3, Sigma, Mu)
    X1 = [x1;x2;x3];
    sigma11 = Sigma(1:3,1:3);
    sigma21 = Sigma(4,1:3);
    sigma22 = Sigma(4,4);
    mu1 = Mu(1:3);
    mu2 = Mu(4);
    x4hat = mu2 + sigma21*inv(sigma11)*(X1 - mu1);
    sigma_4squared = sigma22 -sigma21*inv(sigma11)*sigma21';
end