clear 
clc

%% Variables
t = 0:.001:10;
n = 0:10;

%% functions
ct1 = exp(j * 2 * pi * 0.1 .* t);
ct2 = exp(j * 2 * pi * 1.1 .* t);

dt1 = exp(j * 2 * pi * 0.1 .* n);
dt2 = exp(j * 2 * pi * 1.1 .* n);

%% Plots

hold on
fig = plot(t, ct1, 'linewidth', 2);
plot(t, ct2, 'linewidth', 2);

stem(n, dt1,'linewidth',2);
stem(n, dt2,'linewidth', 2);
xlabel("t/n")
ylabel("x(t)/x[n]")
title("CT & DT CE")
legend("e^j2pi0.1t","e^j2pi1.1t","e^j2pi0.1n","e^j2pi1.1n")
str = "Problem_7.png";
saveas(fig, str);
hold off
