function [xhat,Sigma22] = predictx24(x1, x3, Sigma, Mu)
    X1 = [x1;x3];
    sigma11 = Sigma(1:2:3,1:2:3);
    sigma21 = Sigma(1:2:3,2:2:4);
    sigma22 = Sigma(2:2:4,2:2:4);
    mu1 = Mu(1:2:3);
    mu2 = Mu(2:2:4);
    xhat = mu2 + sigma21*inv(sigma11)*(X1 - mu1);
    Sigma22 = sigma22 -sigma21*inv(sigma11)*sigma21';
end