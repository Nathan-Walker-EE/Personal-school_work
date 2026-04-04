clear 
clc

%% Variables
F = -2:.001:2;
%% Problems 
%Problem 1
% 
% xmag = abs(sin(10 * pi .* F)./(pi .* F));
% xlog = 20.*log(xmag);
% fig = subplot(2, 1, 1);
% plot(F,xmag, 'Linewidth', .2);
% title("Magnitude P1");
% xlabel("F");
% ylabel("|X(F)|");
% subplot(2, 1, 2);
% plot(F,xlog, 'Linewidth', .2);
% title("20log|X(F)| P1");
% xlabel("F");
% ylabel("20log|X(F)|");
% saveas(fig, 'Problem_1.pdf')

%Problem 2
% 
% xmag = abs(exp(-j * 10 * pi .* F) .* (sin(10 * pi .* F)./(pi .* F)));
% xlog = 20.*log(xmag);
% fig = subplot(2, 1, 1);
% plot(F,xmag, 'Linewidth', .2);
% title("Magnitude P2");
% xlabel("F");
% ylabel("|X(F)|");
% subplot(2, 1, 2);
% plot(F,xlog, 'Linewidth', .2);
% title("20log|X(F)| P2");
% xlabel("F");
% ylabel("20log|X(F)|");
% saveas(fig, 'Problem_2.pdf')

%Problem 3
% 
% x = (sin(20 * pi .* F)./(pi .* F)) - (sin(10 * pi .* F)./(pi .* F));
% 
% xmag = abs(x);
% xlog = 20.*log(xmag);
% fig = subplot(2, 1, 1);
% plot(F,xmag, 'Linewidth', .2);
% title("Magnitude P3");
% xlabel("F");
% ylabel("|X(F)|");
% subplot(2, 1, 2);
% plot(F,xlog, 'Linewidth', .2);
% title("20log|X(F)| P3");
% xlabel("F");
% ylabel("20log|X(F)|");
% saveas(fig, 'Problem_3.pdf')

%Problem 4
% 
% x = (exp(j * pi .* F)) .* (sin(10 * pi .* F)./(sin(pi .* F)));
% xmag = abs(x);
% xlog = 20.*log(xmag);
% fig = subplot(2, 1, 1);
% plot(F,xmag, 'Linewidth', .2);
% title("Magnitude 4");
% xlabel("F");
% ylabel("|X(F)|");
% subplot(2, 1, 2);
% plot(F,xlog, 'Linewidth', .2);
% title("20log|X(F)| P4");
% xlabel("F");
% ylabel("20log|X(F)|");
% saveas(fig, 'Problem_4.pdf')

%Problem 5
% 
% x = exp(-j * pi .* F * 9) .* (sin(10 * pi .* F)./(sin(pi .* F)));
% xmag = abs(x);
% xlog = 20.*log(xmag);
% fig = subplot(2, 1, 1);
% plot(F,xmag, 'Linewidth', .2);
% title("Magnitude P5");
% xlabel("F");
% ylabel("|X(F)|");
% subplot(2, 1, 2);
% plot(F,xlog, 'Linewidth', .2);
% title("20log|X(F)| P5");
% xlabel("F");
% ylabel("20log|X(F)|");
% saveas(fig, 'Problem_5.pdf')

% Problem 6
% 
% x = exp(j * pi .* F) .* ((sin(20 * pi .* F) - sin(10 * pi .* F))./(sin(pi .* F)));
% xmag = abs(x);
% xlog = 20.*log(xmag);
% fig = subplot(2, 1, 1);
% plot(F,xmag, 'Linewidth', .2);
% title("Magnitude P6");
% xlabel("F");
% ylabel("|X(F)|");
% subplot(2, 1, 2);
% plot(F,xlog, 'Linewidth', .2);
% title("20log|X(F)| P6");
% xlabel("F");
% ylabel("20log|X(F)|");
% saveas(fig, 'Problem_6.pdf')

