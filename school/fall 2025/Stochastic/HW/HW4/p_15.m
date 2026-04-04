clear all; clc; close all; format compact;

y = 0:.01:1;
sigma = [1, 1.5, 2, 2.5];

figure(1);
hold on;
for i = 1:4
    S = sigma(i);
    f = S./(2*sqrt(2*pi).*y.*(1.-y));
    temp = exp((S^2 * log((1./y) - 1) - 1).^2 ...
        / (-2*S^2));
    f = f .* temp;
    % figure(i)
    plot(abs(f));
end
legend(arrayfun(@(s) sprintf('\\sigma = %.1f', s), ...
    sigma, 'UniformOutput', false));